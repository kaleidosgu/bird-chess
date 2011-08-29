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

CSocketSlot::CSocketSlot()
{
    m_nFd = -1;
    m_nSlotIndex = 0;
    m_next = NULL;

    m_pMsgCache = NULL;
    m_nSeek = 0;
    
    m_pSendDataTail = m_aSendData;
    m_pSendDataHead = m_aSendData;
    m_State = SocketState_Closed;
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

void CSocketSlot::_SetRecvQueue(CRecvDataElementQueue * pRecvQueue)
{
    m_pRecvQueue = pRecvQueue;
}

unsigned int CSocketSlot::GetSlotIndex() const
{
    return m_nSlotIndex;
}

int CSocketSlot::GetFd() const
{
    return m_nFd;
}

SocketState CSocketSlot::GetState() const
{
    return m_State;
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

    MSG_SYSTEM_ConnectSuccess connectSuccessMsg;
    _AddRecvMsg(connectSuccessMsg);
}

bool CSocketSlot::_AddSendMsg(MSG_BASE & rMsg)
{
    CSendDataElement *ptr = m_pSendDataTail;
    ptr++;
    if (ptr == m_aSendData + cMAX_SEND_BUFF_PACKET_COUNT)
    {
        ptr = m_aSendData;
    }
    if (ptr != m_pSendDataHead)
    {
        MSG_BASE * pMsg = CreateDynamicLengthMsg(rMsg.nSize, (MSG_BASE *)0);
        if (pMsg == NULL)
        {
            return false;
        }
        else
        {
            memcpy(pMsg, &rMsg, rMsg.nSize);
            m_pSendDataTail->SetData(pMsg);
            m_pSendDataTail = ptr;
            return true;
        }
    }
    else
    {
        WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_AddSendMsg. Send queue is full. State = %d.\n", m_nSlotIndex, m_State);
        return false;
    }
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
            if (ptr == m_aSendData + cMAX_SEND_BUFF_PACKET_COUNT)
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
    MSG_SYSTEM_Disconnect disconnectMsg;
    return _AddRecvMsg(disconnectMsg);
}

bool CSocketSlot::SendMsg(MSG_BASE & rMsg, bool & rbRemainData)
{
    rbRemainData = false;
    if (m_State != SocketState_Normal)
    {
        WriteLog(LEVEL_DEBUG, "CSocketSlot(%d)::SendMsg. The socket state is not normal. State = %d.\n", m_nSlotIndex, m_State);
        return false;
    }
    else
    {
        return _SendMsg(rMsg, rbRemainData);
    }
}

bool CSocketSlot::_SendMsg(MSG_BASE & rMsg, bool & rbRemainData)
{
    if (m_State != SocketState_Normal && m_State != SocketState_Accepting)
    {
        WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::SendMsg. The socket state is not normal. State = %d.\n", m_nSlotIndex, m_State);
        return false;
    }

    bool bRes = true;
    Mutex::ScopedLock lock(m_MutexForSend);
    if (m_bRemainData)
    {
        bRes = _AddSendMsg(rMsg);
    }
    else
    {
        ssize_t nLen = send(m_nFd, (const void *)&rMsg, rMsg.nSize, 0);

        // nLen<0 is same with nLen==0
        if (nLen < 0)
        {
            bRes = _AddSendMsg(rMsg);
            m_bRemainData = true;
        }
        else if (nLen == 0)
        {
            bRes = _AddSendMsg(rMsg);
            m_bRemainData = true;
        }
        else if (nLen == rMsg.nSize)
        {
        }
        else
        {
            MSG_BASE * pMsg = CreateDynamicLengthMsg(rMsg.nSize, (MSG_BASE *)0);
            if (pMsg == NULL)
            {
                Disconnect(DisconnectReason_CreateMSG_BASE);
                bRes = false;
            }
            else
            {
                memcpy(pMsg, &rMsg, rMsg.nSize);

                m_pMsgCache = pMsg;
                m_nSeek = nLen;
                m_bRemainData = true;
            }
        }
    }
    rbRemainData = m_bRemainData;
    return bRes;
}

bool CSocketSlot::SendData()
{
    if (m_State != SocketState_Normal && m_State != SocketState_Accepting)
    {
        WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::SendData. The socket state is not normal. State = %d.\n", m_nSlotIndex, m_State);
        return false;
    }

    bool bRes = true;
    Mutex::ScopedLock lock(m_MutexForSend);
    if (!m_bRemainData)
    {
        WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::SendData. There is no remain data.\n", m_nSlotIndex);
        return true;
    }
    if (m_pMsgCache == NULL && m_nSeek == 0 && m_pSendDataHead == m_pSendDataTail)
    {
        WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::SendData. There is something wrong with the remain data.\n", m_nSlotIndex);
        _SetState(m_State == SocketState_Accepting ? SocketState_Accepting : SocketState_Normal, SocketState_Broken, DisconnectReason_RemainDataError);
    }
    else if ((m_pMsgCache != NULL && m_nSeek == 0) || (m_pMsgCache == NULL && m_nSeek != 0))
    {
        WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::SendData. There is something wrong with the remain data.\n", m_nSlotIndex);
        _SetState(m_State == SocketState_Accepting ? SocketState_Accepting : SocketState_Normal, SocketState_Broken, DisconnectReason_RemainDataError);
    }
    else
    {
        if (m_pMsgCache != NULL && m_nSeek != 0)
        {
            bRes = _SendMsgCache();
        }

        // the msg cache has been disposed
        if (bRes)
        {
            m_nSeek = 0;

            delete m_pMsgCache;
            m_pMsgCache = NULL;
            bRes = _SendMsgQueue();
        }
        if (bRes)
        {
            m_bRemainData = false;
        }
    }
    return true;
}
bool CSocketSlot::_SendMsgCache()
{
    if (m_State != SocketState_Normal && m_State != SocketState_Accepting)
    {
        WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_SendMsgCache. The socket state is not normal. State = %d.\n", m_nSlotIndex, m_State);
        return false;
    }

    bool bRes = false;
    int nLen = send(m_nFd, ((const char *)m_pMsgCache) + m_nSeek, m_pMsgCache->nSize - m_nSeek, 0);
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
        // any problem, may be some one will deal with it
    }
    else if (nLen == m_pMsgCache->nSize - m_nSeek)
    {
        m_nSeek += nLen;
        bRes = true;
    }
    else
    {
        m_nSeek += nLen;
    }
    return bRes;
}
bool CSocketSlot::_SendMsgQueue()
{
    if (m_State != SocketState_Normal && m_State != SocketState_Accepting)
    {
        WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_SendMsgQueue. The socket state is not normal. State = %d.\n", m_nSlotIndex, m_State);
        return false;
    }

    bool bRes = true;
    CSendDataElement * pTailPtr = m_pSendDataTail;
    int nCount = pTailPtr - m_pSendDataHead;
    if (nCount > 0)
    {
        bRes = _SendElement(m_pSendDataHead, nCount);
    }
    else if (nCount < 0)
    {
        nCount = m_aSendData + cMAX_SEND_BUFF_PACKET_COUNT - m_pSendDataHead;
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

bool CSocketSlot::_SendElement(CSendDataElement *pOut, int nCount)
{
    if (m_State != SocketState_Normal && m_State != SocketState_Accepting)
    {
        WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_SendElement. The socket state is not normal. State = %d.\n", m_nSlotIndex, m_State);
        return false;
    }

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
    }
    else if (nLen == nTotoalSize)
    {
        for (int j=0; j<nCount; ++j)
        {
            pOut[j].DeleteData();
        }
        m_pSendDataHead = m_aSendData + ((m_pSendDataHead - m_aSendData) + nCount)%cMAX_SEND_BUFF_PACKET_COUNT;
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
            m_pSendDataHead = m_aSendData + (pOut - m_aSendData + k)%cMAX_SEND_BUFF_PACKET_COUNT;
        }
        else
        {
            m_pSendDataHead = m_aSendData + (pOut - m_aSendData + k + 1)%cMAX_SEND_BUFF_PACKET_COUNT;
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

    m_bRemainData = false;

    memset(m_Buffer, 0, cMAX_PACKET_SIZE);
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
bool CSocketSlot::_CopyMsgToRecvBuff(MSG_BASE & rMsg)
{
    bool bSuccess = m_pRecvQueue->PutRecvDataElement(rMsg, m_nSlotIndex);
    if (!bSuccess)
    {
        WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_CopyMsgToRecvBuff. Receive queue is full. CurMsg=(%d, %d).\n", m_nSlotIndex, rMsg.nMsg, rMsg.nSize);
    }
    return bSuccess;
}

bool CSocketSlot::_AddRecvMsg(MSG_BASE & rMsg)
{
    bool bDisposeFinish = true;
    // do some thing

    bDisposeFinish = _CopyMsgToRecvBuff(rMsg);
    return bDisposeFinish;
}

bool CSocketSlot::_DisposeMsgData()
{
    if (m_State != SocketState_Normal && m_State != SocketState_Accepting)
    {
        WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_DisposeMsgData. The socket state is not normal. State = %d.\n", m_nSlotIndex, m_State);
        return false;
    }

    bool bRes = true;
    char * pPos = m_Buffer;

    while (m_nBytesRemain >= sizeof(MSG_BASE))
    {
        MSG_BASE *pMsg = (MSG_BASE *)pPos;

        // if Size is Larger than Max Size, Error
        if (pMsg->nSize > cMAX_PACKET_SIZE)
        {
            _SetState(m_State == SocketState_Accepting ? SocketState_Accepting : SocketState_Normal, SocketState_Broken, DisconnectReason_ErrMsgSize);
            WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_DisposeMsgData ErrMsgSize: too large. Size(%d), MaxSize(%d).\n", m_nSlotIndex, pMsg->nSize, cMAX_PACKET_SIZE);
            bRes = false;
            break;
        }

        if (m_nBytesRemain < pMsg->nSize)
        {
            break;
        }

        if (pMsg->nSize < sizeof(MSG_BASE))
        {
            _SetState(m_State == SocketState_Accepting ? SocketState_Accepting : SocketState_Normal, SocketState_Broken, DisconnectReason_ErrMsgSize);
            WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_DisposeMsgData ErrMsgSize: too small. Size(%d), MinSize(%d).\n", m_nSlotIndex, pMsg->nSize, sizeof(MSG_BASE));
            bRes = false;
            break;
        }

        if (_AddRecvMsg(*pMsg))
        {
            m_nBytesRemain -= pMsg->nSize;
            pPos += pMsg->nSize;
        }
        else
        {
            if (cMAX_PACKET_SIZE <= m_nBytesRemain)
            {
                _SetState(m_State == SocketState_Accepting ? SocketState_Accepting : SocketState_Normal, SocketState_Broken, DisconnectReason_RecvBuffOverflow);
                WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_DisposeMsgData. Receive buffee is full.\n", m_nSlotIndex);
                bRes = false;
            }
            break;
        }
    }
    if (m_nBytesRemain > 0 && pPos != m_Buffer)
    {
        memmove(m_Buffer, pPos, m_nBytesRemain);
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
        nLen = recv(m_nFd, m_Buffer + m_nBytesRemain, cMAX_PACKET_SIZE - m_nBytesRemain, 0);
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
            if (m_nBytesRemain >= sizeof(MSG_BASE))
            {
                if (!_DisposeMsgData())
                {
                    bRes = false;
                    break;
                }
            }
        }

    }
    return bRes;
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

bool CSocketSlotMgr::Init(unsigned int nMaxSlotNum, CRecvDataElementQueue * pRecvQueue)
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

