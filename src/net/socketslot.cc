#include "socketslot.h"
#include "systemmsg.h"
#include "socketmgr.h"
#include "../base/log.h"
#include "../base/assert.h"
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string>
#include <netinet/tcp.h>
#include <zlib.h>
#include "socketutil.h"



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

bool net::AddEvent(int nEpollFd, unsigned int nEvents, int nFd, CSocketSlot * pSocketSlot)
{
    int nTmpFd = nFd;
    struct epoll_event ev;
    ev.events = nEvents;
    if (pSocketSlot != NULL)
    {
        ev.data.ptr = pSocketSlot;
        nTmpFd = pSocketSlot->GetFd();
    }
    else
    {
        ev.data.fd = nFd;
    }
    if (epoll_ctl(nEpollFd, EPOLL_CTL_ADD, nTmpFd, &ev) < 0)
    {
        WriteLog(LEVEL_ERROR, "epoll add event failed: fd=%d.\n", nTmpFd);
        return false;
    }
    return true;
}

bool net::ModifyEvent(int nEpollFd, unsigned int nEvents, CSocketSlot * pSocketSlot)
{
    if (pSocketSlot == NULL)
    {
        WriteLog(LEVEL_ERROR, "epoll set modification failed. pSocketSlot == NULL.\n");
        return false;
    }
    struct epoll_event ev;
    ev.events = nEvents;
    ev.data.ptr = pSocketSlot;
    if (epoll_ctl(nEpollFd, EPOLL_CTL_MOD, pSocketSlot->GetFd(), &ev) < 0)
    {
        WriteLog(LEVEL_ERROR, "epoll set modification failed: fd=%d.\n", pSocketSlot->GetFd());
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

    m_pMsgCache = NULL;
    m_nSeek = 0;

    m_pSendDataTail = m_aSendData;
    m_bIsInSendQueue = false;
    m_pSendDataHead = m_aSendData;
    m_State = SocketState_Closed;
    m_pLastBuffer = &m_RecvBuffer[cMAX_RECV_BUFFER_SIZE - cMAX_MSG_SIZE];
    m_tLatestAliveTime = time(NULL);
    //m_pSocketMgr = NULL;
    m_nEpollFd = -1;
    m_pRecvQueue = NULL;
    m_bEncrypt = false;
    m_bCompress = false;
    m_pSendDataBuffer = NULL;
    m_pUncompressBuffer = NULL;
    _Reset();
}

CSocketSlot::~CSocketSlot()
{
    m_next = NULL;
}
void CSocketSlot::_SetEncrypt(bool bEncrypt)
{
    m_bEncrypt = bEncrypt;
}
void CSocketSlot::_SetCompress(bool bCompress)
{
    m_bCompress = bCompress;
}
void CSocketSlot::_SetSendDataBuffer(unsigned char * pSendDataBuffer)
{
    m_pSendDataBuffer = pSendDataBuffer;
}
void CSocketSlot::_SetUncompressBuffer(unsigned char * pUncompressBuffer)
{
    m_pUncompressBuffer = pUncompressBuffer;
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

    /*
    // Add SocketConnectSuccessMsg to Recv queue
    MSG_SYSTEM_SocketConnectSuccess * pSCSMsg = new MSG_SYSTEM_SocketConnectSuccess();
    if (!_AddRecvMsg(pSCSMsg))
    {
    WriteLog(LEVEL_ERROR, "CSocketSlot(%d)::OnAccept. Add Connect success msg failed.\n", m_nSlotIndex);
    delete pSCSMsg;
    pSCSMsg = NULL;
    }
     */
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

/*
   void CSocketSlot::SetStateConnected()
   {
   _SetState(SocketState_Accepting, SocketState_Normal);
   }
 */

bool CSocketSlot::Close()
{
    if (m_nFd != -1)
    {
        close(m_nFd);
        m_nFd = -1;
        /*
           MSG_SYSTEM_SocketDisconnect * pSDMsg = new MSG_SYSTEM_SocketDisconnect();
           if (!_AddRecvMsg(pSDMsg))
           {
           WriteLog(LEVEL_ERROR, "CSocketSlot(%d)::Close. Add Socket Disconnect Msg Failed.\n");
           delete pSDMsg;
           pSDMsg = NULL;
           }
         */
    }
    // m_State = SocketState_Accepting(EPOLLERR), SocketState_Normal(EPOLLERR), SocketState_Disconnecting, SocketState_Broken
    /*
       SocketState eState = SocketState_Broken;
       if (m_State == SocketState_Accepting)
       {
       eState = SocketState_Accepting;
       }
       else if (m_State == SocketState_Disconnecting)
       {
       eState = SocketState_Disconnecting;
       }
     */
    m_MutexForState.Lock();
    WriteLog(LEVEL_DEBUG, "CSocketSlot(%d)::Close. State %d -> %d.\n", m_nSlotIndex, m_State, SocketState_Closed);
    m_State = SocketState_Closed;
    m_MutexForState.Unlock();
    if (m_bConnectSuccess)
    {
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
    return true;
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
    if (rMsg.nSize > cMAX_MSG_SIZE)
    {
        WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::SendMsg. The rMsg.nSize(%d) > cMAX_MSG_SIZE.\n", m_nSlotIndex);
        return false;
    }
    if (m_State != SocketState_Normal)
    {
        WriteLog(LEVEL_DEBUG, "CSocketSlot(%d)::SendMsg. The socket state is not normal. State = %d.\n", m_nSlotIndex, m_State);
        return false;
    }
    // TODO memory pool
    MSG_BASE * pMsg = CreateDynamicLengthMsg(rMsg.nSize, (MSG_BASE *)0);
    memcpy(pMsg, &rMsg, rMsg.nSize);

    if (m_SendQueue.Push(pMsg))
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

bool CSocketSlot::_EncryptDataToWSQ(const unsigned char * pData, int nLen)
{
    MSG_SYSTEM_Encrypted * pEncryptedMsg = CreateDynamicLengthMsg(nLen + sizeof(MSG_SYSTEM_Encrypted) - 1, (MSG_SYSTEM_Encrypted *)0);
    memcpy(pEncryptedMsg->data, pData, nLen);
    pEncryptedMsg->nCheckSum = CheckSum(pData, nLen);

    unsigned char szEncryptBuffer[65530] = {0};
    memcpy(szEncryptBuffer, (unsigned char *)pEncryptedMsg + sizeof(MSG_BASE), pEncryptedMsg->nSize - sizeof(MSG_BASE));
    m_EncryptRC4.Encrypt(szEncryptBuffer, pEncryptedMsg->nSize - sizeof(MSG_BASE), (unsigned char *)(pEncryptedMsg) + sizeof(MSG_BASE));

    //net::Encrypt((unsigned char *)pEncryptedMsg + sizeof(MSG_BASE), pEncryptedMsg->nSize - sizeof(MSG_BASE));

    return _AddMsgToWSQ(pEncryptedMsg);
}

bool CSocketSlot::_AddDataToWSQ(const unsigned char * pData, int nLen)
{
    const unsigned char * pHeader = pData;
    MSG_BASE * pMsg = NULL;
    MSG_BASE * pHeaderMsg = NULL;
    while (pHeader - pData < nLen)
    {
        pHeaderMsg = (MSG_BASE *)pHeader;
        ASSERT(pHeaderMsg->nSize <= cMAX_MSG_SIZE);
        pMsg = CreateDynamicLengthMsg(pHeaderMsg->nSize, (MSG_BASE *)0);
        if (!_AddMsgToWSQ(pMsg))
        {
            return false;
        }
        pMsg = NULL;
        pHeader += pHeaderMsg->nSize;
    }
    if (pHeader - pData != nLen)
    {
        WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_AddDataToWSQ. pHeader(%p) - pData(%p) != nLen(%d).\n", m_nSlotIndex, pHeader, pData, nLen);
        return false;
    }
    else
    {
        return true;
    }
}
bool CSocketSlot::_CompressDataToWSQ(const unsigned char * pData, int nLen)
{
    unsigned long nNewLen = compressBound(nLen);
    ASSERT(nNewLen <= cMAX_COMPRESSED_DATA_SIZE_BOUND);

    MSG_SYSTEM_Compressed * pCompressedMsg = CreateDynamicLengthMsg(nNewLen + sizeof(MSG_SYSTEM_Compressed) - 1, (MSG_SYSTEM_Compressed *)0);
    pCompressedMsg->nCheckSum = CheckSum(pData, nLen);
    int nRet = compress(pCompressedMsg->data, &nNewLen, pData, nLen);
    if (nRet == Z_OK)
    {
        pCompressedMsg->nSize = nNewLen + sizeof(MSG_SYSTEM_Compressed) - 1;
        pCompressedMsg->nSrcDataSize = nLen;
        return _AddMsgToWSQ(pCompressedMsg);
    }
    else
    {
        WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_CompressDataToWSQ. Compress error. nRet=%d, nLen=%lu, nNewLen=%lu.\n", m_nSlotIndex, nRet, nLen, nNewLen);
        delete pCompressedMsg;
        pCompressedMsg = NULL;
        return false;
    }
    /*
       else
       {
       WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_CompressDataToWSQ. Compress error. nRet=%d, nLen=%lu, nNewLen=%lu.\n", m_nSlotIndex, nRet, nLen, nNewLen);
       delete pCompressedMsg;
       pCompressedMsg = NULL;

    // if only one msg
    const unsigned char * pHeader = pData;
    MSG_BASE * pHeaderMsg = (MSG_BASE *)pHeader;
    if (pHeaderMsg->nSize == nLen)
    {
    if (!_AddMsgToWSQ(pHeaderMsg))
    {
    return false;
    }
    else
    {
    rbNotDelete = true;
    return true;
    }
    }

    MSG_BASE * pMsg = NULL;
    while (pHeader - pData < nLen)
    {
    pHeaderMsg = (MSG_BASE *)pHeader;
    pMsg = CreateDynamicLengthMsg(pHeaderMsg->nSize, (MSG_BASE *)0);
    if (!_AddMsgToWSQ(pMsg))
    {
    pMsg = NULL;
    return false;
    }
    pMsg = NULL;
    pHeader += pHeaderMsg->nSize;
    }
    if (pHeader - pData != nLen)
    {
    WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_CompressDataToWSQ. pData is error. pHeader=%p, pData=%p, nLen=%lu.\n", m_nSlotIndex, pHeader, pData, nLen);
    return false;
    }
    }
    return true;
     */
}
bool CSocketSlot::_CompressAndEncryptDataToWSQ(const unsigned char * pData, int nLen)
{
    unsigned long nNewLen = compressBound(nLen);
    ASSERT(nNewLen <= cMAX_COMPRESSED_DATA_SIZE_BOUND);

    MSG_SYSTEM_CompressedAndEncrypted * pCEMsg = CreateDynamicLengthMsg(nNewLen + sizeof(MSG_SYSTEM_CompressedAndEncrypted) - 1, (MSG_SYSTEM_CompressedAndEncrypted *)0);
    pCEMsg->nCheckSum = CheckSum(pData, nLen);
    int nRet = compress(pCEMsg->data, &nNewLen, pData, nLen);
    if (nRet == Z_OK)
    {
        pCEMsg->nSize = nNewLen + sizeof(MSG_SYSTEM_CompressedAndEncrypted) - 1;
        pCEMsg->nSrcDataSize = nLen;

        unsigned char szEncryptBuffer[65530] = {0};
        memcpy(szEncryptBuffer, (unsigned char *)pCEMsg + sizeof(MSG_BASE), pCEMsg->nSize - sizeof(MSG_BASE));
        m_EncryptRC4.Encrypt(szEncryptBuffer, pCEMsg->nSize - sizeof(MSG_BASE), (unsigned char *)(pCEMsg) + sizeof(MSG_BASE));

        //net::Encrypt((unsigned char *)pCEMsg + sizeof(MSG_BASE), pCEMsg->nSize - sizeof(MSG_BASE));
        return _AddMsgToWSQ(pCEMsg);
    }
    else
    {
        WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_CompressAndEncryptDataToWSQ. Compress error. nRet=%d, nLen=%lu, nNewLen=%lu.\n", m_nSlotIndex, nRet, nLen, nNewLen);
        delete pCEMsg;
        pCEMsg = NULL;
        return false;
    }
}

const unsigned int cMIN_SIZE_FOR_COMPRESS = 100;
bool CSocketSlot::_DisposeSendQueue()
{
    // get and deal with the msg which is in send queue
    MSG_BASE * pMsg = NULL;
    if (m_bCompress)
    {
        while (true)
        {
            unsigned int nTotalSize = 0;
            int nCount = 0;
            MSG_BASE * pFirstMsg = NULL;

            // dispose the msgs which is in send queue
            unsigned char * pCBHeader = m_pSendDataBuffer;
            while (m_SendQueue.Front(pMsg))
            {
                if (nTotalSize + pMsg->nSize <= cMAX_SEND_DATA_BUFFER_SIZE)
                {
                    nTotalSize += pMsg->nSize;
                    memcpy(pCBHeader, pMsg, pMsg->nSize);
                    pCBHeader += pMsg->nSize;
                    m_SendQueue.Pop(pMsg);
                    if (nCount == 0)
                    {
                        pFirstMsg = pMsg;
                    }
                    else
                    {
                        delete pMsg;
                        pMsg = NULL;
                    }
                    nCount++;
                }
                else
                {
                    if (nCount == 0)
                    {
                        pFirstMsg = pMsg;
                        m_SendQueue.Pop(pMsg);
                        nCount++;
                    }
                    break;
                }
            }


            // send data
            if (nCount > 1)
            {
                // if not only one msg, do with m_pSendDataBuffer by nTotalSize
                delete pFirstMsg;
                pFirstMsg = NULL;
                if (m_bEncrypt)
                {
                    if (!_CompressAndEncryptDataToWSQ(m_pSendDataBuffer, nTotalSize))
                    {
                        if (!_EncryptDataToWSQ(m_pSendDataBuffer, nTotalSize))
                        {
                            return false;
                        }
                    }
                }
                else
                {
                    if (!_CompressDataToWSQ(m_pSendDataBuffer, nTotalSize))
                    {
                        if (!_AddDataToWSQ(m_pSendDataBuffer, nTotalSize))
                        {
                            return false;
                        }
                    }
                }
            }
            else if (nCount == 1)
            {
                // if only one msg
                if (pFirstMsg->nSize < cMIN_SIZE_FOR_COMPRESS)
                {
                    // need not compress
                    if (m_bEncrypt)
                    {
                        if (!_EncryptDataToWSQ((unsigned char *)pFirstMsg, pFirstMsg->nSize))
                        {
                            delete pFirstMsg;
                            pFirstMsg = NULL;
                            return false;
                        }
                        delete pFirstMsg;
                        pFirstMsg = NULL;
                    }
                    else
                    {
                        if (!_AddMsgToWSQ(pFirstMsg))
                        {
                            return false;
                        }
                        pFirstMsg = NULL;
                    }
                }
                else
                {
                    // need compress
                    if (m_bEncrypt)
                    {
                        if (_CompressDataToWSQ((const unsigned char *)pFirstMsg, pFirstMsg->nSize))
                        {
                            if (!_AddMsgToWSQ(pFirstMsg))
                            {
                                return false;
                            }
                            pFirstMsg = NULL;
                        }
                        else
                        {
                            delete pFirstMsg;
                            pFirstMsg = NULL;
                        }
                    }
                    else
                    {
                        if (!_CompressAndEncryptDataToWSQ((unsigned char *)pFirstMsg, pFirstMsg->nSize))
                        {
                            if (!_EncryptDataToWSQ((unsigned char *)pFirstMsg, pFirstMsg->nSize))
                            {
                                delete pFirstMsg;
                                pFirstMsg = NULL;
                                return false;
                            }
                        }
                        delete pFirstMsg;
                        pFirstMsg = NULL;
                    }
                }
            }
            else
            {
                break;
            }
        }
    }
    else
    {
        if (m_bEncrypt)
        {
            while (m_SendQueue.Pop(pMsg))
            {
                if (!_EncryptDataToWSQ((unsigned char *)pMsg, pMsg->nSize))
                {
                    delete pMsg;
                    pMsg = NULL;
                    return false;
                }
                delete pMsg;
                pMsg = NULL;
            }
        }
        else
        {
            while (m_SendQueue.Pop(pMsg))
            {
                if (!_AddMsgToWSQ(pMsg))
                {
                    return false;
                }
                pMsg = NULL;
            }
        }
    }
    return true;
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

bool CSocketSlot::_AddMsgToWSQ(MSG_BASE * pMsg)
{
    ASSERT(pMsg != NULL);

    CSendDataElement *ptr = m_pSendDataTail;
    ptr++;
    if (ptr == m_aSendData + cSEND_QUEUE_SIZE)
    {
        ptr = m_aSendData;
    }
    if (ptr != m_pSendDataHead)
    {
        // can add
        m_pSendDataTail->SetData(pMsg);
        m_pSendDataTail = ptr;
        pMsg = NULL;
        return true;
    }
    else
    {
        // can't add
        WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_AddMsgToWSQ. The waiting send queue is full. MSGID=%d.\n", m_nSlotIndex, pMsg->nMsg);
        delete pMsg;
        pMsg = NULL;
        return false;
    }
}
bool CSocketSlot::_SendMsgDirectly(MSG_BASE * pMsg)
{
    ASSERT(pMsg != NULL);

    if (!_AddMsgToWSQ(pMsg))
    {
        return false;
    }
    pMsg = NULL;

    if (!SendData())
    {
        if (m_State == SocketState_Accepting || m_State == SocketState_Normal)
        {
            // Modify event
            /*
            if (m_pSocketMgr)
            {
                WriteLog(LEVEL_DEBUG, "CSocketSlot(%d)::_SendMsgDirectly. The system buffer is full.\n", m_nSlotIndex);
                m_pSocketMgr->_ModifyEvent(EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLET, this);
            }
            */
            WriteLog(LEVEL_DEBUG, "CSocketSlot(%d)::_SendMsgDirectly. The system buffer is full.\n", m_nSlotIndex);
            ModifyEvent(m_nEpollFd, EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLET, this);
        }
        else
        {
            return false;
        }
    }
    return true;
}

bool CSocketSlot::SendData()
{
    if (m_State != SocketState_Normal && m_State != SocketState_Accepting)
    {
        WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::SendData. The socket state is not normal. State = %d.\n", m_nSlotIndex, m_State);
        return false;
    }

    // dispose with the send queue.
    if (!_DisposeSendQueue())
    {
        _SetState(m_State == SocketState_Accepting ? SocketState_Accepting : SocketState_Normal, SocketState_Broken, DisconnectReason_DisposeSendQueueFailed);
        return false;
    }

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
        /*
           MSG_SYSTEM_SocketDisconnect * pSDMsg = new MSG_SYSTEM_SocketDisconnect();
           if (!_AddRecvMsg(pSDMsg))
           {
           WriteLog(LEVEL_ERROR, "CSocketSlot(%d)::_Reset. Add Socket Disconnect Msg Failed.\n");
           delete pSDMsg;
           pSDMsg = NULL;
           }
         */
    }

    m_bConnectSuccess = false;
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
/*
   void CSocketSlot::_Pretreat(MSG_BASE * &pMsg)
   {
   switch (pMsg->nMsg)
   {
   case MSGID_SYSTEM_ClientPublicKey:
   {
// TODO : Get secret key, and send to client
m_EncryptKey = "";
m_DecryptKey = "";
MSG_SYSTEM_SecretKey * pSKMsg = new MSG_SYSTEM_SecretKey();
_SendMsgDirectly(pSKMsg);
delete pMsg;
pMsg = NULL;

MSG_SYSTEM_ConnectSuccess * pCSMsg = new MSG_SYSTEM_ConnectSuccess();
if (!_AddRecvMsg(pCSMsg))
{
WriteLog(LEVEL_ERROR, "CSocketSlot(%d)::_Pretreat. Add Connect success msg failed.\n", m_nSlotIndex);
delete pCSMsg;
pCSMsg = NULL;
}
else
{
m_bConnectSuccess = true;
}
}
break;
case MSGID_SYSTEM_UseNewSecretKey:
{
// TODO: update new key
m_DecryptKey = m_EncryptKey;
delete pMsg;
pMsg = NULL;
}
break;
case MSGID_SYSTEM_CheckAliveReply:
{
m_tLatestAliveTime = time(NULL);
delete pMsg;
pMsg = NULL;
}
break;
default:
break;
}
}
 */

bool CSocketSlot::_AddRecvMsg(MSG_BASE * pMsg)
{
    ASSERT(pMsg != NULL);

    // check alive, as the check alive reply msg may be compressed and encrypted, so check it here
    if (pMsg->nMsg == MSGID_SYSTEM_CheckAliveReply)
    {
        m_tLatestAliveTime = time(NULL);
        WriteLog(LEVEL_DEBUG, "CSocketSlot(%d)::_AddRecvMsg. Get the check alive reply msg, time=%u.\n", m_nSlotIndex, (unsigned int)m_tLatestAliveTime);
        delete pMsg;
        pMsg = NULL;
        return true;
    }

    /*
       _Pretreat(pMsg);
       if (pMsg == NULL)
       {
       return true;
       }
     */
    if (m_State != SocketState_Normal && m_State != SocketState_Closed)
    {
        WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_AddRecvMsg. The state(%d) is normal or closed. MSGID=%d.\n", m_nSlotIndex, m_State, pMsg->nMsg);
        return false;
    }

    CRecvDataElement * pRecvDataElement = new CRecvDataElement();
    pRecvDataElement->SetData(pMsg, m_nSlotIndex);
    if (m_pRecvQueue->Push(pRecvDataElement))
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
/*
   void CSocketSlot::_Uncompress(unsigned char * pDes, unsigned int nNewLen, const unsigned char * pSrc, unsigned int nLen)
   {
   }
 */
void CSocketSlot::_GenerateSecretKey()
{
    //memcpy(m_EncryptKey, "kjkl!@#$ADFWQ$R%", cMAX_SECRETKEY_LEN);
}

bool CSocketSlot::_AddRecvData(const unsigned char * pData, int nLen)
{
    const unsigned char * pHeader = pData;
    MSG_BASE * pMsg = NULL;
    while (pHeader < pData + nLen)
    {
        MSG_BASE * pHeaderMsg = (MSG_BASE *)pHeader;
        pMsg = CreateDynamicLengthMsg(pHeaderMsg->nSize, (MSG_BASE *)0);
        memcpy(pMsg, pHeaderMsg, pHeaderMsg->nSize);
        if (!_AddRecvMsg(pMsg))
        {
            WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_AddRecvData. Add Recv msg failed.\n", m_nSlotIndex);
            delete pMsg;
            pMsg = NULL;
            return false;
        }
        pMsg = NULL;
        pHeader += pHeaderMsg->nSize;
    }
    if (pHeader - pData != nLen)
    {
        WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_AddRecvData. pHeader(%p) - pData(%p) != nLen(%d).\n", m_nSlotIndex, pHeader, pData, nLen);
        return false;
    }
    else
    {
        return true;
    }
}
bool CSocketSlot::_DisposeRecvMsg(MSG_BASE & rMsg)
{
    switch (rMsg.nMsg)
    {
        case MSGID_SYSTEM_ClientPublicKey:
            {
                WriteLog(LEVEL_DEBUG, "CSocketSlot(%d)::_DisposeRecvMsg. Receive the public key.\n", m_nSlotIndex);
                if (rMsg.nSize != sizeof(MSG_SYSTEM_ClientPublicKey))
                {
                    WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_DisposeRecvMsg. sizeof(MSG_SYSTEM_ClientPublicKey) != rMsg.nSize.\n", m_nSlotIndex);
                    return false;
                }

                // get the client public key
                MSG_SYSTEM_ClientPublicKey & rCPKMsg = (MSG_SYSTEM_ClientPublicKey &)rMsg;
                //cMAX_HALF_OF_PUBLIC_KEY_LEN
                unsigned char * szPublicKey = new unsigned char[rCPKMsg.nSrcKeySize1 + rCPKMsg.nSrcKeySize2];
                unsigned char * pPublicKey1 = szPublicKey;
                unsigned char * pPublicKey2 = szPublicKey + rCPKMsg.nSrcKeySize1;
                int nSrcKeySize1 = m_pServerRSA->PrivateDecrypt(rCPKMsg.key1, cMAX_ENCRYPTED_PUBLIC_KEY_LEN, pPublicKey1);
                if (nSrcKeySize1 != rCPKMsg.nSrcKeySize1)
                {
                    WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_DisposeRecvMsg. rCPKMsg.nSrcKeySize1(%d) != nSrcKeySize1(%d).\n", m_nSlotIndex, rCPKMsg.nSrcKeySize1, nSrcKeySize1);
                    return false;
                }
                int nSrcKeySize2 = m_pServerRSA->PrivateDecrypt(rCPKMsg.key2, cMAX_ENCRYPTED_PUBLIC_KEY_LEN, pPublicKey2);
                if (nSrcKeySize2 != rCPKMsg.nSrcKeySize2)
                {
                    WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_DisposeRecvMsg. rCPKMsg.nSrcKeySize2(%d) != nSrcKeySize2(%d).\n", m_nSlotIndex, rCPKMsg.nSrcKeySize2, nSrcKeySize2);
                    return false;
                }

                m_ClientRSA.SetPublicKey(szPublicKey, rCPKMsg.nSrcKeySize1 + rCPKMsg.nSrcKeySize2);
                delete szPublicKey;
                szPublicKey = NULL;
                pPublicKey1 = NULL;
                pPublicKey2 = NULL;

                //_GenerateSecretKey();
                unsigned char szSecretKey[cSECRET_KEY_LEN + 1] = "no";
                //_GenerateSecretKey(szSecretKey);
                m_EncryptRC4.SetKey(szSecretKey, cSECRET_KEY_LEN);
                m_DecryptRC4.SetKey(szSecretKey, cSECRET_KEY_LEN);

                // encrypt secret key
                unsigned char szCEKey[64];
                int nCEKeySize = m_ClientRSA.PublicEncrypt(szSecretKey, cSECRET_KEY_LEN, szCEKey);
                WriteLog(LEVEL_DEBUG, "CSocketSlot(%d)::_DisposeRecvMsg. nCEKeySize=%d.\n", m_nSlotIndex, nCEKeySize);
                ASSERT(nCEKeySize == 64);
                unsigned char szSEKey[128];
                int nSEKeySize1 = m_pServerRSA->PrivateEncrypt(szCEKey, 32, szSEKey);
                WriteLog(LEVEL_DEBUG, "CSocketSlot(%d)::_DisposeRecvMsg. nSEKeySize1=%d.\n", m_nSlotIndex, nSEKeySize1);
                ASSERT(nSEKeySize1 == 64);
                int nSEKeySize2 = m_pServerRSA->PrivateEncrypt(szCEKey + 32, 32, szSEKey+64);
                WriteLog(LEVEL_DEBUG, "CSocketSlot(%d)::_DisposeRecvMsg. nSEKeySize2=%d.\n", m_nSlotIndex, nSEKeySize2);
                ASSERT(nSEKeySize2 == 64);

                // send secret key
                MSG_SYSTEM_S2C_SecretKey * pSKMsg = new MSG_SYSTEM_S2C_SecretKey();
                memcpy(pSKMsg->key, szSEKey, cENCRYPTED_SECRET_KEY_LEN);
                if (!_SendMsgDirectly(pSKMsg))
                {
                    WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_DisposeRecvMsg. Send secret key failed.\n", m_nSlotIndex);
                    return false;
                }
                _SetState(SocketState_Accepting, SocketState_Normal);
                MSG_SYSTEM_ConnectSuccess * pCSMsg = new MSG_SYSTEM_ConnectSuccess();
                if (!_AddRecvMsg(pCSMsg))
                {
                    WriteLog(LEVEL_ERROR, "CSocketSlot(%d)::_DisposeRecvMsg. Add Connect success msg failed.\n", m_nSlotIndex);
                    delete pCSMsg;
                    pCSMsg = NULL;
                    return false;
                }
                else
                {
                    m_bConnectSuccess = true;
                }
            }
            break;
        case MSGID_SYSTEM_CompressedAndEncrypted:
            {
                if (!m_bCompress || !m_bEncrypt)
                {
                    WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_DisposeRecvMsg. Received CompressedAndEncrypted msg. but m_bCompress= %d and m_bEncrypt = %d.\n", m_nSlotIndex, m_bCompress, m_bEncrypt);
                    return false;
                }

                unsigned char szDecryptBuffer[65530] = {0};
                memcpy(szDecryptBuffer, (unsigned char *)(&rMsg) + sizeof(MSG_BASE), rMsg.nSize - sizeof(MSG_BASE));
                m_DecryptRC4.Decrypt(szDecryptBuffer, rMsg.nSize - sizeof(MSG_BASE), (unsigned char *)(&rMsg) + sizeof(MSG_BASE));

                MSG_SYSTEM_CompressedAndEncrypted & rCEMsg = (MSG_SYSTEM_CompressedAndEncrypted &)rMsg;
                if (rCEMsg.nSize > cMAX_MSG_SIZE)
                {
                    WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_DisposeRecvMsg. rCEMsg.nSize(%d) > cMAX_MSG_SIZE(%d).\n", m_nSlotIndex, rCEMsg.nSize, cMAX_MSG_SIZE);
                    return false;
                }

                if (rCEMsg.nSrcDataSize > cMAX_C_AND_E_DATA_SIZE)
                {
                    WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_DisposeRecvMsg. rCEMsg.nSrcDataSize(%d) > cMAX_C_AND_E_DATA_SIZE(%d).\n", m_nSlotIndex, rCEMsg.nSrcDataSize, cMAX_C_AND_E_DATA_SIZE);
                    return false;
                }

                unsigned long nNewLen = rCEMsg.nSrcDataSize;
                int nRet = uncompress(m_pUncompressBuffer, &nNewLen, rCEMsg.data, rCEMsg.nSize - sizeof(MSG_SYSTEM_CompressedAndEncrypted) + 1);
                if (nRet != Z_OK)
                {
                    WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_DisposeRecvMsg. uncompress failed. nRet=%d, nNewLen=%lu, nLen=%lu.\n", m_nSlotIndex, nRet, nNewLen, rCEMsg.nSize - sizeof(MSG_SYSTEM_CompressedAndEncrypted) + 1);
                    return false;
                }

                if (nNewLen != rCEMsg.nSrcDataSize)
                {
                    WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_DisposeRecvMsg. nNewLen(%lu) != rCEMsg.nSrcDataSize(%d).\n", m_nSlotIndex, nNewLen, rCEMsg.nSrcDataSize);
                    return false;
                }
                if (CheckSum(m_pUncompressBuffer, nNewLen) != rCEMsg.nCheckSum)
                {
                    WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_DisposeRecvMsg. CheckSum(m_pUncompressBuffer, nNewLen(%lu)) !=rCEMsg.nCheckSum.\n", m_nSlotIndex, nNewLen);
                    return false;
                }
                if (!_AddRecvData(m_pUncompressBuffer, nNewLen))
                {
                    return false;
                }
            }
            break;
        case MSGID_SYSTEM_Compressed:
            {
                if (!m_bCompress)
                {
                    WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_DisposeRecvMsg. Received Compressed msg. but m_bCompress is false.\n", m_nSlotIndex);
                    return false;
                }
                MSG_SYSTEM_Compressed & rCompressedMsg = (MSG_SYSTEM_Compressed &)rMsg;
                if (rCompressedMsg.nSrcDataSize > cMAX_COMPRESSED_DATA_SIZE)
                {
                    WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_DisposeRecvMsg. rCompressedMsg.nSrcDataSize(%d) > cMAX_COMPRESSED_DATA_SIZE(%d).\n", m_nSlotIndex, rCompressedMsg.nSrcDataSize, cMAX_COMPRESSED_DATA_SIZE);
                    return false;
                }
                unsigned long nNewLen = rCompressedMsg.nSrcDataSize;
                int nRet = uncompress(m_pUncompressBuffer, &nNewLen, rCompressedMsg.data, rCompressedMsg.nSize - sizeof(MSG_SYSTEM_Compressed) + 1);
                if (nRet != Z_OK)
                {
                    WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_DisposeRecvMsg. uncompress failed. nRet=%d, nNewLen=%lu, nLen=%lu.\n", m_nSlotIndex, nRet, nNewLen, rCompressedMsg.nSize - sizeof(MSG_SYSTEM_Compressed) + 1);
                    return false;
                }
                if (nNewLen != rCompressedMsg.nSrcDataSize)
                {
                    WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_DisposeRecvMsg. nNewLen(%lu) != rCompressedMsg.nSrcDataSize(%d).\n", m_nSlotIndex, nNewLen, rCompressedMsg.nSrcDataSize);
                    return false;
                }
                if (CheckSum(m_pUncompressBuffer, nNewLen) != rCompressedMsg.nCheckSum)
                {
                    WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_DisposeRecvMsg. CheckSum(m_pUncompressBuffer, nNewLen(%lu)) !=rCompressedMsg.nCheckSum.\n", m_nSlotIndex, nNewLen);
                    return false;
                }
                if (!_AddRecvData(m_pUncompressBuffer, nNewLen))
                {
                    return false;
                }
            }
            break;
        case MSGID_SYSTEM_Encrypted:
            {
                if (!m_bEncrypt)
                {
                    WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_DisposeRecvMsg. Received Encrypted msg. but m_bEncrypt is false.\n", m_nSlotIndex);
                    return false;
                }
                unsigned char szDecryptBuffer[65530] = {0};
                memcpy(szDecryptBuffer, (unsigned char *)(&rMsg) + sizeof(MSG_BASE), rMsg.nSize - sizeof(MSG_BASE));
                m_DecryptRC4.Decrypt(szDecryptBuffer, rMsg.nSize - sizeof(MSG_BASE), (unsigned char *)(&rMsg) + sizeof(MSG_BASE));
                MSG_SYSTEM_Encrypted & rEncryptedMsg = (MSG_SYSTEM_Encrypted &)rMsg;
                int nDataSize = rEncryptedMsg.nSize - sizeof(MSG_SYSTEM_Encrypted) + 1;
                if (CheckSum(rEncryptedMsg.data, nDataSize) != rEncryptedMsg.nCheckSum)
                {
                    WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_DisposeRecvMsg. CheckSum(rEncryptedMsg.data, nDataSize(%d)) != rEncryptedMsg.nCheckSum.\n", m_nSlotIndex, nDataSize);
                    return false;
                }

                if (!_AddRecvData(rEncryptedMsg.data, nDataSize))
                {
                    return false;
                }
            }
            break;
        default:
            {
                MSG_BASE * pMsg = CreateDynamicLengthMsg(rMsg.nSize, (MSG_BASE *)0);
                memcpy(pMsg, &rMsg, rMsg.nSize);
                if (!_AddRecvMsg(pMsg))
                {
                    WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_DisposeRecvMsg. Add Recv msg failed.\n", m_nSlotIndex);
                    delete pMsg;
                    pMsg = NULL;
                    return false;
                }
            }
            break;
    }
    return true;
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
        MSG_BASE * pMsg = (MSG_BASE *)m_pRecvHead;

        // if Size is Larger than Max Size, Error
        if (pMsg->nSize > cMAX_MSG_SIZE)
        {
            _SetState(m_State == SocketState_Accepting ? SocketState_Accepting : SocketState_Normal, SocketState_Broken, DisconnectReason_ErrMsgSize);
            WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_DisposeRecvBuffer ErrMsgSize: too large. Size(%d), MaxSize(%d).\n", m_nSlotIndex, pMsg->nSize, cMAX_MSG_SIZE);
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
            if (m_State == SocketState_Normal)
            {
                WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_DisposeRecvBuffer. Recv queue is full.\n", m_nSlotIndex);
                _SetState(SocketState_Normal, SocketState_Broken, DisconnectReason_RecvBuffOverflow);
            }
            else
            {
                WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_DisposeRecvBuffer. Recv queue is full or the state(%d) is not normal.\n", m_nSlotIndex, m_State);
                _SetState(SocketState_Accepting, SocketState_Broken, DisconnectReason_RecvBuffOverflow_OR_StateNotNormal);
            }
            bRes = false;
            break;
        }
    }
    if (m_pRecvHead > m_pLastBuffer)
    {
        for (int i=0; i<m_nBytesRemain; i++)
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
        nLen = recv(m_nFd, m_pRecvHead + m_nBytesRemain, cMAX_RECV_BUFFER_SIZE - m_nBytesRemain, 0);
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
    //if (m_State == SocketState_Normal)
    {
        MSG_SYSTEM_CheckAlive * pCheckAliveMsg = new MSG_SYSTEM_CheckAlive();
        if (!m_SendQueue.Push(pCheckAliveMsg))
        {
            WriteLog(LEVEL_ERROR, "CSocketSlot(%d)::_CheckAlive. The send queue is full.\n", m_nSlotIndex);
            delete pCheckAliveMsg;
            pCheckAliveMsg = NULL;
        }
    }
}

/*
   void CSocketSlot::SetStateNotAlive()
   {
   _SetState(SocketState_Normal, SocketState_Disconnecting, DisconnectReason_AliveTimeOut);
   }
 */

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

//bool CSocketSlotMgr::Init(unsigned int nMaxSlotNum, LoopQueue< CRecvDataElement * > * pRecvQueue, CSocketMgr * pSocketMgr, CRSA * pServerRSA, bool bEncrypt, bool bCompress, unsigned char * pSendDataBuffer, unsigned char * pUncompressBuffer)
bool CSocketSlotMgr::Init(unsigned int nMaxSlotNum, LoopQueue< CRecvDataElement * > * pRecvQueue, int nEpollFd, CRSA * pServerRSA, bool bEncrypt, bool bCompress, unsigned char * pSendDataBuffer, unsigned char * pUncompressBuffer)
{
    if (!pRecvQueue)
    {
        WriteLog(LEVEL_ERROR, "CSocketSlotMgr::Init. Receive queue is NULL.\n");
        return false;
    }
    /*
    if (!pSocketMgr)
    {
        WriteLog(LEVEL_ERROR, "CSocketSlotMgr::Init. pSocketMgr is NULL.\n");
        return false;
    }
    */
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
        //m_aSlot[i]._SetSocketMgr(pSocketMgr);
        m_aSlot[i]._SetEpollFd(nEpollFd);
        m_aSlot[i]._SetServerRSA(pServerRSA);
        m_aSlot[i]._SetEncrypt(bEncrypt);
        m_aSlot[i]._SetCompress(bCompress);
        m_aSlot[i]._SetSendDataBuffer(pSendDataBuffer);
        m_aSlot[i]._SetUncompressBuffer(pUncompressBuffer);

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
