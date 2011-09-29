#include "socketslot.h"
#include "systemmsg.h"
#include "../base/log.h"
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string>
#include <netinet/tcp.h>

using namespace net;
using namespace base;

bool net::SetNonBlocking(int nFd)
{
    int flags;
    if ((flags = fcntl(nFd, F_GETFL, NULL)) < 0)
    { 
        WriteLog(LEVEL_WARNING, "fcntl(%d, F_GETFL).\n", nFd);
        return false;
    }    
    if (fcntl(nFd, F_SETFL, flags | O_NONBLOCK) == -1)
    {
        WriteLog(LEVEL_WARNING, "fcntl(%d, F_SETFL).\n", nFd);
        return false;
    }
    return true;
}

CSocketSlot::CSocketSlot() :
    m_SendQueue(cSEND_QUEUE_SIZE)
{
    m_nFd = -1;
    m_nSlotIndex = 0;
    m_next = NULL;

    /*
       m_pBeginWSQ = &m_aWaintingSendQueue[0];
       m_pEndWSQ = &m_aWaintingSendQueue[0];
       m_pHeadWSQ = m_pBeginWSQ;
       m_pTailWSQ = m_pBeginWSQ;
     */

    m_pEncryptFunc = NULL;
    m_pDecryptFunc = NULL;
    m_pCompressFunc = NULL;
    m_pUncompressFunc = NULL;
    m_pMsgCache = NULL;
    m_nSeek = 0;

    m_pSendDataTail = m_aSendData;
    m_bIsInSendQueue = false;
    m_pSendDataHead = m_aSendData;
    m_State = SocketState_Closed;
    m_pLastBuffer = &m_RecvBuffer[cMAX_RECV_BUFFER_SIZE - cMAX_PACKET_SIZE];
    m_tLatestAliveTime = time(NULL);
    _Reset();
}

CSocketSlot::~CSocketSlot()
{
    m_next = NULL;
}
void CSocketSlot::_SetSlotIndex(unsigned int nSlotIndex)
{
    m_nSlotIndex = nSlotIndex;
}

void CSocketSlot::_SetRecvQueue(LoopQueue< CRecvDataElement * > * pRecvQueue)
{
    m_pRecvQueue = pRecvQueue;
}

void CSocketSlot::OnAccept(int nFd, const sockaddr_in &rSaClient)
{
    m_nFd = nFd;
    m_addr = rSaClient.sin_addr;
    m_nPort = ntohs(rSaClient.sin_port);

    SetNonBlocking(m_nFd);

    int nOptVal = 1;
    setsockopt(m_nFd, IPPROTO_TCP, TCP_NODELAY, (void *)&nOptVal, sizeof(int));
    _SetState(SocketState_Free, SocketState_Accepting);

    MSG_SYSTEM_ConnectSuccess * pConnectSuccessMsg = new MSG_SYSTEM_ConnectSuccess();
    if (!_AddRecvMsg(pConnectSuccessMsg))
    {
        WriteLog(LEVEL_ERROR, "CSocketSlot(%d)::OnAccept. Add Connect success msg failed.\n", m_nSlotIndex);
        delete pConnectSuccessMsg;
        pConnectSuccessMsg = NULL;
    }
    m_tLatestAliveTime = time(NULL);
}


void CSocketSlot::_ClearSendMsgQueue()
{
    if (m_pSendDataHead != m_pSendDataTail)
    {
        CSendDataElement *ptr = m_pSendDataHead;
        while (ptr != m_pSendDataTail)
        {
            ptr->DeleteData();
            ++ptr;
            if (ptr == m_aSendData + cSEND_QUEUE_SIZE)
            {
                ptr = m_aSendData;
            }
        }
        m_pSendDataHead = m_aSendData;
        m_pSendDataTail = m_aSendData;
    }
}

bool CSocketSlot::Disconnect(int nDisconnectReason)
{
    if (m_State != SocketState_Normal)
    {
        WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::Disconnect. The socket state is not normal. State = %d.\n", m_nSlotIndex, m_State);
        return false;
    }
    else
    {
        _SetState(SocketState_Normal, SocketState_Disconnecting, nDisconnectReason);
        return true;
    }
}

void CSocketSlot::SetStateConnected()
{
    _SetState(SocketState_Accepting, SocketState_Normal);
}

bool CSocketSlot::Close()
{
    if (m_nFd != -1)
    {
        close(m_nFd);
        m_nFd = -1;
    }
    SocketState eState = SocketState_Broken;
    if (m_State == SocketState_Accepting)
    {
        eState = SocketState_Accepting;
    }
    else if (m_State == SocketState_Disconnecting)
    {
        eState = SocketState_Disconnecting;
    }
    m_MutexForState.Lock();
    WriteLog(LEVEL_DEBUG, "CSocketSlot(%d)::Close. State %d -> %d.\n", m_nSlotIndex, m_State, SocketState_Closed);
    m_State = SocketState_Closed;
    m_MutexForState.Unlock();
    MSG_SYSTEM_Disconnect * pDisconnectMsg = new MSG_SYSTEM_Disconnect();
    if (_AddRecvMsg(pDisconnectMsg))
    {
        return true;
    }
    else
    {
        WriteLog(LEVEL_ERROR, "CSocketSlot(%d)::Close. Add Disconnect Msg Failed.\n");
        delete pDisconnectMsg;
        pDisconnectMsg = NULL;
        return false;
    }
}

void CSocketSlot::SetInSendQueue()
{
    Mutex::ScopedLock lock(m_MutexForIsInSendQueue);
    m_bIsInSendQueue = true;
}

void CSocketSlot::SetNotInSendQueue()
{
    Mutex::ScopedLock lock(m_MutexForIsInSendQueue);
    m_bIsInSendQueue = false;
}

bool CSocketSlot::SendMsg(MSG_BASE & rMsg)
{
    if (m_State != SocketState_Normal)
    {
        WriteLog(LEVEL_DEBUG, "CSocketSlot(%d)::SendMsg. The socket state is not normal. State = %d.\n", m_nSlotIndex, m_State);
        return false;
    }
    // TODO memory pool
    MSG_BASE * pMsg = CreateDynamicLengthMsg(rMsg.nSize, (MSG_BASE *)0);
    memcpy(pMsg, &rMsg, rMsg.nSize);

    if (m_SendQueue.AddElement(pMsg))
    {
        return true;
    }
    else
    {
        WriteLog(LEVEL_ERROR, "CSocketSlot(%d)::SendMsg. The send queue is full.\n", m_nSlotIndex);
        delete pMsg;
        pMsg = NULL;
        return false;
    }
}

void CSocketSlot::_DisposeSendQueue()
{
    // get and deal with the msg which is in send queue
    MSG_BASE * pMsg = NULL;
    if (m_pEncryptFunc == NULL && m_pCompressFunc == NULL)
    {
        while (true)
        {
            CSendDataElement *ptr = m_pSendDataTail;
            ptr++;
            if (ptr == m_aSendData + cSEND_QUEUE_SIZE)
            {
                ptr = m_aSendData;
            }
            if (ptr != m_pSendDataHead)
            {
                // can add
                if (m_SendQueue.GetElement(pMsg))
                {
                    m_pSendDataTail->SetData(pMsg);
                    pMsg = NULL;
                    m_pSendDataTail = ptr;
                }
                else
                {
                    break;
                }
            }
            else
            {
                // can't add
                WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_DisposeSendQueue. The waiting send queue is full .\n", m_nSlotIndex);
                return;
            }
        }
    }
    else
    {
        while (true)
        {
            CSendDataElement *ptr = m_pSendDataTail;
            ptr++;
            if (ptr == m_aSendData + cSEND_QUEUE_SIZE)
            {
                ptr = m_aSendData;
            }
            if (ptr != m_pSendDataHead)
            {
                MSG_BASE * pNewMsg = NULL;
                // can add
                if (m_SendQueue.GetElement(pMsg))
                {
                    pNewMsg = pMsg;
                    pMsg = NULL;
                }
                else
                {
                    break;
                }
                if (pNewMsg)
                {
                    m_pSendDataTail->SetData(pNewMsg);
                    m_pSendDataTail = ptr;
                }
            }
            else
            {
                // can't add
                WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_DisposeSendQueue. The waiting send queue is full .\n", m_nSlotIndex);
                return;
            }
        }
    }
}

bool CSocketSlot::_SendBufferData()
{
    /*
       if (m_State != SocketState_Normal && m_State != SocketState_Accepting)
       {
       WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_SendBufferData. The socket state is not normal. State = %d.\n", m_nSlotIndex, m_State);
       return false;
       }
     */

    if ((m_pMsgCache != NULL && m_nSeek == 0) || (m_pMsgCache == NULL && m_nSeek != 0))
    {
        WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_SendBufferData. There is something wrong with the remain data.\n", m_nSlotIndex);
        _SetState(m_State == SocketState_Accepting ? SocketState_Accepting : SocketState_Normal, SocketState_Broken, DisconnectReason_RemainDataError);
        return false;
    }

    if (m_pMsgCache == NULL)
    {
        return true;
    }

    bool bRes = false;
    int nLen = send(m_nFd, ((const char *)m_pMsgCache) + m_nSeek, m_pMsgCache->nSize - m_nSeek, 0);
    if (nLen == m_pMsgCache->nSize - m_nSeek)
    {
        delete m_pMsgCache;
        m_pMsgCache = NULL;
        m_nSeek = 0;
        bRes = true;
    }
    else if (nLen < 0)
    {
        int nErr = errno;
        if (nErr != EAGAIN)
        {
            _SetState(m_State == SocketState_Accepting ? SocketState_Accepting : SocketState_Normal, SocketState_Broken, DisconnectReason_SendError);
        }
    }
    else if (nLen == 0)
    {
        // any problem, may be some one will deal with it
        WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_SendBufferData. There is not remain data. Remain msg size = (%d), Seek=(%d).", m_nSlotIndex, m_pMsgCache->nSize, m_nSeek);
    }
    else
    {
        m_nSeek += nLen;
    }
    return bRes;
}

bool CSocketSlot::_SendWSQ()
{
    /*
       if (m_State != SocketState_Normal && m_State != SocketState_Accepting)
       {
       WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_SendMsgQueue. The socket state is not normal. State = %d.\n", m_nSlotIndex, m_State);
       return false;
       }
     */

    bool bRes = true;
    CSendDataElement * pTailPtr = m_pSendDataTail;
    int nCount = pTailPtr - m_pSendDataHead;
    if (nCount > 0)
    {
        bRes = _SendElement(m_pSendDataHead, nCount);
    }
    else if (nCount < 0)
    {
        nCount = m_aSendData + cSEND_QUEUE_SIZE - m_pSendDataHead;
        if (nCount > 0)
        {
            bRes = _SendElement(m_pSendDataHead, nCount);
        }
        nCount = pTailPtr - m_aSendData;
        if (nCount > 0 && bRes)
        {
            bRes = _SendElement(m_aSendData, nCount);
        }
    }
    return bRes;
}

bool CSocketSlot::SendData()
{
    if (m_State != SocketState_Normal && m_State != SocketState_Accepting)
    {
        WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::SendData. The socket state is not normal. State = %d.\n", m_nSlotIndex, m_State);
        return false;
    }

    // dispose with the send queue.
    _DisposeSendQueue();

    // send buffer data
    if (!_SendBufferData())
    {
        return false;
    }

    // send waiting send queue
    return _SendWSQ();
}

bool CSocketSlot::_SendElement(CSendDataElement *pOut, int nCount)
{
    /*
       if (m_State != SocketState_Normal && m_State != SocketState_Accepting)
       {
       WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_SendElement. The socket state is not normal. State = %d.\n", m_nSlotIndex, m_State);
       return false;
       }
     */

    bool bRes = false;
    int nTotoalSize = 0;
    for (int i=0; i<nCount; ++i)
    { 
        nTotoalSize += pOut[i].GetMsgDataSize();
    }
    int nLen = writev(m_nFd, pOut->GetIovec(), nCount);
    if (nLen < 0)
    {
        int nErr = errno;
        if (nErr == EAGAIN)
        {
        }
        else
        {
            _SetState(m_State == SocketState_Accepting ? SocketState_Accepting : SocketState_Normal, SocketState_Broken, DisconnectReason_SendError);
        }
    }
    else if (nLen == 0)
    {
        WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_SendElement. No data has been sent. TotoalSize=%d, Count=%d.\n", m_nSlotIndex, nTotoalSize, nCount);
    }
    else if (nLen == nTotoalSize)
    {
        for (int j=0; j<nCount; ++j)
        {
            pOut[j].DeleteData();
        }
        m_pSendDataHead = m_aSendData + ((m_pSendDataHead - m_aSendData) + nCount)%cSEND_QUEUE_SIZE;
        bRes = true;
    }
    else
    {
        int nPos = 0;
        int k=0;
        for (k=0; k<nCount; ++k)
        {
            nPos += pOut[k].GetMsgDataSize();
            if (nPos > nLen)
            {
                break;
            }
            pOut[k].DeleteData();
        }
        m_pMsgCache = (MSG_BASE *)pOut[k].GetMsgData();
        m_nSeek = nLen + pOut[k].GetMsgDataSize() - nPos;

        if (m_nSeek == 0)
        {
            m_pMsgCache = NULL;
            m_pSendDataHead = m_aSendData + (pOut - m_aSendData + k)%cSEND_QUEUE_SIZE;
        }
        else
        {
            m_pSendDataHead = m_aSendData + (pOut - m_aSendData + k + 1)%cSEND_QUEUE_SIZE;
        }
    }
    return bRes;
}

void CSocketSlot::_Reset()
{
    if (m_nFd != -1)
    {
        close(m_nFd);
        m_nFd = -1;
    }

    //m_bRemainData = false;

    memset(m_RecvBuffer, 0, cMAX_RECV_BUFFER_SIZE);
    m_pRecvHead = m_RecvBuffer;
    m_nBytesRemain = 0;

    if (m_pMsgCache)
    {
        delete m_pMsgCache;
        m_pMsgCache = NULL;
        m_nSeek = 0;
    }
    _SetState(m_State == SocketState_Accepting ? SocketState_Accepting : SocketState_Closed, SocketState_Free, DisconnectReason_Unkown);
}

bool CSocketSlot::_SetState(SocketState eSrcState, SocketState eDesState, int nReason)
{
    bool bRes = _SetState(eSrcState, eDesState);
    if (bRes)
    {
        m_nDisconnectReason = nReason;
    }
    return bRes;
}

bool CSocketSlot::_SetState(SocketState eSrcState, SocketState eDesState)
{
    bool bRes = true;
    Mutex::ScopedLock lock(m_MutexForState);
    if (m_State == eSrcState)
    {
        m_State = eDesState;
        //WriteLog(LEVEL_DEBUG, "CSocketSlot(%d)::_SetState. State %d -> %d.\n", m_nSlotIndex, eSrcState, eDesState);
    }
    else
    {
        WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_SetState. Set socket state failed. Current state(%d), src state(%d), des state(%d).\n", m_nSlotIndex, m_State, eSrcState, eDesState);
        bRes = false;
    }

    return bRes;
} 

bool CSocketSlot::_AddRecvMsg(MSG_BASE * pMsg)
{
    if (pMsg == NULL)
    {
        return false;
    }

    CRecvDataElement * pRecvDataElement = new CRecvDataElement();
    pRecvDataElement->SetData(pMsg, m_nSlotIndex);
    if (m_pRecvQueue->AddElement(pRecvDataElement))
    {
        return true;
    }
    else
    {
        WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_AddRecvMsg. Receive queue is full. CurMsg=(%d, %d).\n", m_nSlotIndex, pMsg->nMsg, pMsg->nSize);
        delete pRecvDataElement;
        pRecvDataElement = NULL;
        return false;
    }
}

bool CSocketSlot::_DisposeRecvMsg(MSG_BASE & rMsg)
{
    bool bRes = true;
    MSG_BASE * pMsg = NULL;
    if (m_pDecryptFunc == NULL && m_pUncompressFunc == NULL)
    {
        pMsg = CreateDynamicLengthMsg(rMsg.nSize, (MSG_BASE *)0);
        memcpy(pMsg, &rMsg, rMsg.nSize);
        if (!_AddRecvMsg(pMsg))
        {
            WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_DisposeRecvMsg. Add Recv msg failed.\n", m_nSlotIndex);
            delete pMsg;
            pMsg = NULL;
            bRes = false;
        }
    }
    else
    {
    }
    return bRes;
}

bool CSocketSlot::_DisposeRecvBuffer()
{
    /*
       if (m_State != SocketState_Normal && m_State != SocketState_Accepting)
       {
       WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_DisposeRecvBuffer. The socket state is not normal. State = %d.\n", m_nSlotIndex, m_State);
       return false;
       }
     */

    bool bRes = true;
    while (m_nBytesRemain >= sizeof(MSG_BASE))
    {
        MSG_BASE *pMsg = (MSG_BASE *)m_pRecvHead;

        // if Size is Larger than Max Size, Error
        if (pMsg->nSize > cMAX_PACKET_SIZE)
        {
            _SetState(m_State == SocketState_Accepting ? SocketState_Accepting : SocketState_Normal, SocketState_Broken, DisconnectReason_ErrMsgSize);
            WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_DisposeRecvBuffer ErrMsgSize: too large. Size(%d), MaxSize(%d).\n", m_nSlotIndex, pMsg->nSize, cMAX_PACKET_SIZE);
            bRes = false;
            break;
        }
        else if (pMsg->nSize < sizeof(MSG_BASE))
        {
            _SetState(m_State == SocketState_Accepting ? SocketState_Accepting : SocketState_Normal, SocketState_Broken, DisconnectReason_ErrMsgSize);
            WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_DisposeRecvBuffer ErrMsgSize: too small. Size(%d), MinSize(%d).\n", m_nSlotIndex, pMsg->nSize, sizeof(MSG_BASE));
            bRes = false;
            break;
        }

        if (m_nBytesRemain < pMsg->nSize)
        {
            break;
        }

        if (_DisposeRecvMsg(*pMsg))
        {
            m_nBytesRemain -= pMsg->nSize;
            m_pRecvHead += pMsg->nSize;
        }
        else
        {
            WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_DisposeRecvBuffer. Recv queue is full.\n", m_nSlotIndex);
            _SetState(m_State == SocketState_Accepting ? SocketState_Accepting : SocketState_Normal, SocketState_Broken, DisconnectReason_RecvBuffOverflow);
            bRes = false;
            break;
        }
    }
    if (m_pRecvHead > m_pLastBuffer)
    {
        for (unsigned int i=0; i<m_nBytesRemain; i++)
        {
            m_RecvBuffer[i] = m_pRecvHead[i];
        }
        m_pRecvHead = m_RecvBuffer;
    }
    return bRes;
}

bool CSocketSlot::RecvData()
{
    if (m_State != SocketState_Normal && m_State != SocketState_Accepting)
    {
        WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::RevData. The socket state is not normal. State = %d.\n", m_nSlotIndex, m_State);
        return false;
    }

    bool bRes = true;

    int nLen = 0;
    while (true)
    {
        nLen = recv(m_nFd, m_pRecvHead + m_nBytesRemain, cMAX_PACKET_SIZE - m_nBytesRemain, 0);
        if (nLen <0)
        {
            int nErr = errno;
            if (nErr == EAGAIN)
            {
            }
            else
            {
                _SetState(m_State == SocketState_Accepting ? SocketState_Accepting : SocketState_Normal, SocketState_Broken, DisconnectReason_ReadError);
                WriteLog(LEVEL_DEBUG, "CSocketSlot(%d)::RecvData. Read error. ErrInfo=(%s); m_nBytesRemain = %d.\n", m_nSlotIndex, strerror(nErr), m_nBytesRemain);
                bRes = false;
            }
            break;
        }
        else if (nLen == 0)
        {
            _SetState(m_State == SocketState_Accepting ? SocketState_Accepting : SocketState_Normal, SocketState_Broken, DisconnectReason_Shutdown);
            bRes = false;
            break;
        }
        else
        {
            m_nBytesRemain += nLen;
            bRes = _DisposeRecvBuffer();
            if (!bRes)
            {
                break;
            }
        }

    }
    return bRes;
}

void CSocketSlot::SendAliveMsg()
{
    //if (m_State == SocketState_Normal || m_State == SocketState_Accepting)
    {
        MSG_SYSTEM_CheckAlive * pCheckAliveMsg = new MSG_SYSTEM_CheckAlive();
        if (!m_SendQueue.AddElement(pCheckAliveMsg))
        {
            WriteLog(LEVEL_ERROR, "CSocketSlot(%d)::_CheckAlive. The send queue is full.\n", m_nSlotIndex);
            delete pCheckAliveMsg;
            pCheckAliveMsg = NULL;
        }
    }
}

void CSocketSlot::SetStateNotAlive()
{
    _SetState(m_State == SocketState_Accepting ? SocketState_Accepting : SocketState_Normal, SocketState_Disconnecting, DisconnectReason_AliveTimeOut);
}

CSocketSlotMgr::CSocketSlotMgr()
{
    m_nMaxSlotNum = 0;
    m_nFreeSlotNum = 0;
    m_aSlot = NULL;
    m_pFreeSlot = NULL;
}
CSocketSlotMgr::~CSocketSlotMgr()
{
    delete [] m_aSlot;
    m_aSlot = NULL;
    m_pFreeSlot = NULL;
}

bool CSocketSlotMgr::Init(unsigned int nMaxSlotNum, LoopQueue< CRecvDataElement * > * pRecvQueue)
{
    if (!pRecvQueue)
    {
        WriteLog(LEVEL_ERROR, "CSocketSlotMgr::Init. Receive queue is NULL.\n");
        return false;
    }
    m_nMaxSlotNum = nMaxSlotNum;
    m_aSlot = new CSocketSlot[m_nMaxSlotNum];
    if (m_aSlot == NULL)
    {
        WriteLog(LEVEL_WARNING, "Init SocketSlotMgr Failed.\n");
        return false;
    }
    for (int i=m_nMaxSlotNum -1; i>=0; --i)
    {
        m_aSlot[i].m_next = m_pFreeSlot;
        m_aSlot[i]._SetSlotIndex(i);
        m_aSlot[i]._SetRecvQueue(pRecvQueue);
        m_pFreeSlot = &m_aSlot[i];
    }
    m_nFreeSlotNum = m_nMaxSlotNum;
    return true;
}

CSocketSlot * CSocketSlotMgr::GetFreeSlot()
{
    Mutex::ScopedLock lock(m_MutexForFreeSlotQueue);
    CSocketSlot * pFreeSlot = m_pFreeSlot;
    if (pFreeSlot != NULL)
    {
        m_pFreeSlot = m_pFreeSlot->m_next;
        --m_nFreeSlotNum;

        // mark as not free
        pFreeSlot->m_next = pFreeSlot;
    }
    return pFreeSlot;
}

bool CSocketSlotMgr::ReleaseSlot(unsigned int nSlotIndex)
{
    CSocketSlot * pSocketSlot = GetSocketSlot(nSlotIndex);
    if (pSocketSlot == NULL)
    {
        return false;
    }
    return ReleaseSlot(*pSocketSlot);
}

bool CSocketSlotMgr::ReleaseSlot(CSocketSlot & rSocketSlot)
{
    rSocketSlot._Reset();
    return _FreeSlot(rSocketSlot);
}

CSocketSlot * CSocketSlotMgr::GetSocketSlot(unsigned int nSlotIndex) const
{
    if (nSlotIndex >= m_nMaxSlotNum)
    {
        WriteLog(LEVEL_WARNING, "MaxSlotNum(%d) is less that SlotIndex(%d).\n", m_nMaxSlotNum, nSlotIndex);
        return NULL;
    }
    else
    {
        return &m_aSlot[nSlotIndex];
    }
}

//unsigned int CSocketSlotMgr::GetCurrentSlotNum() const
//{
//    unsigned int nCount = 0;
//    for (unsigned int i = 0; i < m_nMaxSlotNum; ++i)
//    {
//        if (GetSocketSlot(i) != NULL && GetSocketSlot(i)->IsValidSocket())
//        {
//            ++nCount;
//        }
//    }
//    return nCount;
//}

bool CSocketSlotMgr::_FreeSlot(CSocketSlot & rSocketSlot)
{
    if (rSocketSlot.m_next != &rSocketSlot)
    {
        WriteLog(LEVEL_WARNING, "This slot has been released. SlotIndex=%d.\n", rSocketSlot.m_nSlotIndex);
        return false;
    }
    Mutex::ScopedLock lock(m_MutexForFreeSlotQueue);
    rSocketSlot.m_next = m_pFreeSlot;
    m_pFreeSlot = &rSocketSlot;
    ++m_nFreeSlotNum;
    return true;
}
