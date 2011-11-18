#include "socketmgr.h"
#include "recvdataelement.h"
#include "systemmsg.h"
#include "../base/log.h"
#include "../base/ini.h"
#include <sys/epoll.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>

const unsigned int cMAX_SPARE_FD = 32;

const int cLISTEN_QUEUE_SIZE = 16;

const unsigned int cRECV_QUEUE_SIZE = 65536;

using namespace net;

extern bool SetNonBlocking(int nFd);
extern bool ModifyEvent(int nEpollFd, unsigned int nEvents, CSocketSlot * pSocketSlot);
extern bool AddEvent(int nEpollFd, unsigned int nEvents, int nFd, CSocketSlot * pSocketSlot);

void SetFileLimit(unsigned int nMaxClient)
{
    struct rlimit rlim;
    getrlimit(RLIMIT_NOFILE, &rlim);
    unsigned int nSoftLimit = rlim.rlim_cur;
    if (nMaxClient > rlim.rlim_max - cMAX_SPARE_FD)
    {
        WriteLog(LEVEL_WARNING, "Hard Limit is less than MaxClient. Hard Limit = %d.\n", rlim.rlim_max);
        rlim.rlim_max = nMaxClient + cMAX_SPARE_FD;
    }
    if (nMaxClient > rlim.rlim_cur - cMAX_SPARE_FD)
    {
        WriteLog(LEVEL_INFO, "Soft Limit is less than MaxClient. Soft Limit = %d.\n", rlim.rlim_cur);
        rlim.rlim_cur = nMaxClient + cMAX_SPARE_FD;
    }
    if (setrlimit(RLIMIT_NOFILE, &rlim) != 0)
    {
        WriteLog(LEVEL_WARNING, "Set Limit Failed. errno = %d.\n", errno);
        nMaxClient = nSoftLimit - cMAX_SPARE_FD;
    }

}

CSocketMgr::CSocketMgr()
{
    m_bInitSuccess = false;
    m_nPort = 0;
    m_nListenerFd = -1;
    m_nMaxClient = 0;
    m_bGetMsgThreadSafety = false;
    m_bRunning = false;
    m_pAddSendQueue = &m_SendQueue1;
    m_pDelSendQueue = &m_SendQueue2;
    m_nAliveCheckInterval = 0;
    m_nAliveTimeOut = 30;
    m_tLastCheckTime = time(NULL);
    m_bEncrypt = false;
    m_bCompress = false;
    memset(m_SendDataBuffer, 0, cMAX_SEND_DATA_BUFFER_SIZE);
    memset(m_UncompressBuffer, 0, cMAX_COMPRESSED_DATA_SIZE);

    srand(time(NULL)); // for rand the secret key.
}

CSocketMgr::~CSocketMgr()
{
    Stop();
    if (m_nListenerFd != -1)
    {
        close(m_nListenerFd);
        m_nListenerFd = -1;
    }
}

/*
   bool CSocketMgr::_AddEvent(int nFd, unsigned int nEvents, CSocketSlot * pSocketSlot)
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
   if (epoll_ctl(m_nEpollFd, EPOLL_CTL_ADD, nTmpFd, &ev) < 0)
   {
   WriteLog(LEVEL_ERROR, "epoll add event failed: fd=%d.\n", nTmpFd);
   return false;
   }
   return true;
   }
 */

/*
   bool CSocketMgr::_ModifyEvent(unsigned int nEvents, CSocketSlot * pSocketSlot)
   {
   if (pSocketSlot == NULL)
   {
   WriteLog(LEVEL_ERROR, "epoll set modification failed. pSocketSlot == NULL.\n");
   return false;
   }
   struct epoll_event ev;
   ev.events = nEvents;
   ev.data.ptr = pSocketSlot;
   if (epoll_ctl(m_nEpollFd, EPOLL_CTL_MOD, pSocketSlot->GetFd(), &ev) < 0)
   {
   WriteLog(LEVEL_ERROR, "epoll set modification failed: fd=%d.\n", pSocketSlot->GetFd());
   return false;
   }
   return true;
   }
 */

bool CSocketMgr::Init(unsigned short nPort, unsigned int nMaxClient, bool bEncrypt, bool bCompress, unsigned int nAliveCheckInterval, unsigned int nAliveTimeOut, bool bGetMsgThreadSafety)
{
    if (nPort == 0)
    {
        return false;
    }
    if (nMaxClient == 0)
    {
        return false;
    }
    m_bEncrypt = bEncrypt;
    m_bCompress = bCompress;
    m_nAliveCheckInterval = nAliveCheckInterval;
    m_nAliveTimeOut = nAliveTimeOut;
    if (m_nAliveCheckInterval > 0 && m_nAliveTimeOut <= m_nAliveCheckInterval)
    {
        m_nAliveTimeOut = m_nAliveCheckInterval + 1;
    }
    m_bGetMsgThreadSafety = bGetMsgThreadSafety;
    m_nPort = nPort;
    m_nMaxClient = nMaxClient;

    // Set MaxClient is large than 32
    if (m_nMaxClient < 32)
    {
        m_nMaxClient = 32;
    }

    string strn = "";
    string stre = "";
    string strd = "";
    string strp = "";
    string strq = "";
    string strdmp1 = "";
    string strdmq1 = "";
    string stripmq = "";
    GIni.GetString("Encrypt", "n", strn, "00AA36ABCE88ACFDFF55523C7FC4523F90EFA00DF3774A259F2E62B4C5D99CB5ADB300A0285E5301930E0C70FB6876939CE616CE624A11E0086D341EBCACA0A1F5");
    GIni.GetString("Encrypt", "e", stre, "11");
    GIni.GetString("Encrypt", "d", strd, "0A033748626487695F5F30BC38B98B44C2CD2DFF434098CD20D8A138D090BF64797C3FA7A2CDCB3CD1E0BDBA2654B4F9DF8E8AE59D733D9F33B301624AFD1D51");

    GIni.GetString("Encrypt", "p", strp, "00D840B41666B42E92EA0DA3B43204B5CFCE3352524D0416A5A441E700AF46120D");
    GIni.GetString("Encrypt", "q", strq, "00C97FB1F027F453F6341233EAAAD1D9353F6C42D08866B1D05A0F2035028B9D89");
    GIni.GetString("Encrypt", "dmp1", strdmp1, "590B9572A2C2A9C406059DC2AB2F1DAFEB7E8B4F10A7549E8EEDF5B4FCE09E05");
    GIni.GetString("Encrypt", "dmq1", strdmq1, "008E3C0521FE15E0EA06A36FF0F10C9952C35B7A7514FD3238B80AAD5298628D51");
    GIni.GetString("Encrypt", "ipmq", stripmq, "363FF7189DA8E90B1D341F71D09B76A8A943E11D10B24D249F2DEAFEF80C1826");

    m_ServerRSA.InitPrivateKey(strn.c_str(), stre.c_str(), strd.c_str(), strp.c_str(), strq.c_str(), strdmp1.c_str(), strdmq1.c_str(), stripmq.c_str());
    if (_InitServer())
    {
        m_bInitSuccess = true;
        return true;
    }
    else
    {
        WriteLog(LEVEL_ERROR, "Init Socket Server Failed.\n");
        return false;
    }
}

bool CSocketMgr::_InitServer()
{
    /*
       if (m_nPort == 0 || m_nMaxClient == 0)
       {
       WriteLog(LEVEL_ERROR, "You have not init the socketmgr.\n");
       return false;
       }
     */

    // create epoll
    m_nEpollFd = epoll_create(m_nMaxClient);

    //m_RecvQueue.Init(cRECV_QUEUE_SIZE);
    //m_RecvArray.Init(cRECV_QUEUE_SIZE);
    m_pRecvQueue = new LoopQueue< CRecvDataElement * >(cRECV_QUEUE_SIZE);
    //m_SocketSlotMgr.Init(m_nMaxClient, m_pRecvQueue, this, &m_ServerRSA, m_bEncrypt, m_bCompress, m_SendDataBuffer, m_UncompressBuffer);
    m_SocketSlotMgr.Init(m_nMaxClient, m_pRecvQueue, m_nEpollFd, &m_ServerRSA, m_bEncrypt, m_bCompress, m_SendDataBuffer, m_UncompressBuffer);

    SetFileLimit(m_nMaxClient);

    //socket
    struct sockaddr_in saServer;
    if (m_nListenerFd != -1)
    {
        close(m_nListenerFd);
        m_nListenerFd = -1;
    }
    m_nListenerFd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_nListenerFd < 0)
    {
        WriteLog(LEVEL_ERROR, "Socket Failed.\n");
        return false;
    }

    // set non blocking
    SetNonBlocking(m_nListenerFd);

    // set reuse address
    int optval = 1;
    setsockopt(m_nListenerFd, SOL_SOCKET, SO_REUSEADDR, (void *)&optval, sizeof(optval));

    saServer.sin_family = AF_INET;
    saServer.sin_addr.s_addr = INADDR_ANY;
    saServer.sin_port = htons(m_nPort);

    // add listener fd to epoll
    //if (!_AddEvent(m_nListenerFd, EPOLLIN | EPOLLET | EPOLLRDHUP, NULL))
    if (!AddEvent(m_nEpollFd, EPOLLIN | EPOLLET | EPOLLRDHUP, m_nListenerFd, NULL))
    {
        WriteLog(LEVEL_ERROR, "Add Listener FD to Epoll Failed. FD=%d.\n", m_nListenerFd);
        return false;
    }

    //bind
    int nRes = bind(m_nListenerFd, (sockaddr*)&saServer, sizeof(saServer));
    if (nRes != 0)
    {
        WriteLog(LEVEL_ERROR, "Bind to Port Failed. Port = %d.\n", m_nPort);
        return false;
    }

    //listen
    nRes = listen(m_nListenerFd, cLISTEN_QUEUE_SIZE);
    if (nRes != 0)
    {
        WriteLog(LEVEL_ERROR, "Listen On Port Failed. Port = %d.\n", m_nPort);
        return false;
    }
    return true;
}

void CSocketMgr::_SendData()
{
    vector< CSocketSlot * >::iterator it;
    for (it=m_pDelSendQueue->begin(); it!=m_pDelSendQueue->end(); ++it)
    {
        CSocketSlot * pSocketSlot = *it;
        if (pSocketSlot)
        {
            pSocketSlot->SetNotInSendQueue();
            if (!pSocketSlot->SendData())
            {
                // if the system buffer is full
                if (pSocketSlot->GetState() == SocketState_Accepting || pSocketSlot->GetState() == SocketState_Normal)
                {
                    WriteLog(LEVEL_DEBUG, "The system buffer is full. SlotIndex=%d.\n", pSocketSlot->GetSlotIndex());
                    //_ModifyEvent(EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLET, pSocketSlot);
                    ModifyEvent(m_nEpollFd, EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLET, pSocketSlot);
                }
            }
            /*
            // socket is borken
            else
            {
            }
             */
        }
    }
    m_pDelSendQueue->clear();
}

void CSocketMgr::Process()
{
    if (!m_bInitSuccess)
    {
        WriteLog(LEVEL_ERROR, "You have not init the socketmgr or init the socketmgr failed.\n");
        return;
    }


    m_tLastCheckTime = time(NULL);

    m_bRunning = true;
    int nfds = 0;
    struct epoll_event * events = new epoll_event[m_nMaxClient];
    bool bBusy = false;
    while (m_bRunning)
    {
        bBusy = false;
        // recv
        nfds = epoll_wait(m_nEpollFd, events, m_nMaxClient, 0);
        if (nfds > 0)
        {
            bBusy = true;
        }
        for(int i = 0; i < nfds; ++i)
        {
            if(events[i].data.fd == m_nListenerFd)
            {
                if (!_AcceptConnection())
                {
                    continue;
                }
            }
            else
            {
                _ProcessEpollEvent(events[i]);
            }
        }

        // send
        m_MutexForSendQueue.Lock();
        vector< CSocketSlot * > * pTmp = m_pAddSendQueue;
        m_pAddSendQueue = m_pDelSendQueue;
        m_pDelSendQueue = pTmp;
        m_MutexForSendQueue.Unlock();

        if (!m_pDelSendQueue->empty())
        {
            _SendData();
            bBusy = true;
        }

        // if not busy, then sleep for a moment.
        if (!bBusy)
        {
            usleep(1000);
        }
    }
    WriteLog("Process Complete.\n");
}

bool CSocketMgr::_AcceptConnection()
{
    struct sockaddr_in saClient;
    socklen_t nSaLen = sizeof(saClient);

    // accept connection
    int nConnectFd = accept(m_nListenerFd, (struct sockaddr *)&saClient, (socklen_t*)&nSaLen);
    if (nConnectFd < 0)
    {
        WriteLog(LEVEL_WARNING, "accept what? what's wrong? fd = %d.\n", nConnectFd);
        return false;
    }
    else
    {
        WriteLog(LEVEL_DEBUG, "connection accept from %s.\n", inet_ntoa(saClient.sin_addr));
    }

    // Be denied ?


    // Get Free Slot
    CSocketSlot * pFreeSlot = m_SocketSlotMgr.GetFreeSlot();
    if (pFreeSlot == NULL)
    {
        WriteLog(LEVEL_WARNING, "Server Full.\n");
        close(nConnectFd);
        return false;
    }
    pFreeSlot->OnAccept(nConnectFd, saClient);

    // add new fd to epoll
    //if (!_AddEvent(nConnectFd, EPOLLIN | EPOLLRDHUP | EPOLLET, pFreeSlot))
    if (!AddEvent(m_nEpollFd, EPOLLIN | EPOLLRDHUP | EPOLLET, nConnectFd, pFreeSlot))
    {
        WriteLog(LEVEL_ERROR, "epoll add event failed. fd=%d.\n", nConnectFd);
        m_SocketSlotMgr.ReleaseSlot(*pFreeSlot);
        return false;
    }
    return true;
}

bool CSocketMgr::_ProcessEpollEvent(struct epoll_event & rEv)
{
    CSocketSlot * pSocketSlot = (CSocketSlot *)(rEv.data.ptr);
    if (pSocketSlot == NULL)
    {
        WriteLog(LEVEL_ERROR, "Cant' Find The CSocketSlot. fd=%d\n", rEv.data.fd);
        return false;
    }

    if (rEv.events & EPOLLERR)
    {
        //WriteLog(LEVEL_DEBUG, "Received EPOLLERR event. Slot=%d\n", pSocketSlot->GetSlotIndex());
        _Close(*pSocketSlot);
        return true;
    }
    if (rEv.events & EPOLLHUP)
    {
        //WriteLog(LEVEL_DEBUG, "Received EPOLLHUP event. Slot=%d\n", pSocketSlot->GetSlotIndex());
        _Close(*pSocketSlot);
        return true;
    }
    if (rEv.events & EPOLLRDHUP)
    {
        //WriteLog(LEVEL_DEBUG, "Received EPOLLRDHUP event. Slot=%d\n", pSocketSlot->GetSlotIndex());
        _Close(*pSocketSlot);
        return true;
    }
    if (rEv.events & EPOLLIN)
    {
        //WriteLog(LEVEL_DEBUG, "Received EPOLLIN event. Slot=%d\n", pSocketSlot->GetSlotIndex());
        if (!pSocketSlot->RecvData())
        {
            WriteLog(LEVEL_DEBUG, "Receive Data Failed. Slot=%d\n", pSocketSlot->GetSlotIndex());
            _Close(*pSocketSlot);
            return true;
        }
        /*
           if (pSocketSlot->GetState() != SocketState_Accepting && pSocketSlot->GetState() != SocketState_Normal)
           {
           WriteLog(LEVEL_DEBUG, "Receive Data Failed. Slot=%d\n", pSocketSlot->GetSlotIndex());
           _Close(*pSocketSlot);
           return true;
           }
           else if (pSocketSlot->GetState() == SocketState_Broken)
           {
           WriteLog(LEVEL_WARNING, "What's wrong with the State. State = %d, Slot=%d\n", pSocketSlot->GetState(), pSocketSlot->GetSlotIndex());
           }
         */
    }
    if (rEv.events & EPOLLOUT)
    {
        //WriteLog(LEVEL_DEBUG, "Received EPOLLOUT event. Slot=%d\n", pSocketSlot->GetSlotIndex());
        if (pSocketSlot->GetState() == SocketState_Disconnecting)
        {
            WriteLog(LEVEL_DEBUG, "The socket's state is disconnecting. Slot=%d\n", pSocketSlot->GetSlotIndex());
            _Close(*pSocketSlot);
            return true;
        }

        if (pSocketSlot->SendData())
        {
            //_ModifyEvent(EPOLLIN | EPOLLRDHUP | EPOLLET, pSocketSlot);
            ModifyEvent(m_nEpollFd, EPOLLIN | EPOLLRDHUP | EPOLLET, pSocketSlot);
        }
        else
        {
            // 1. system buffer is full
            // 2. send error

            // if send error, close
            if (pSocketSlot->GetState() != SocketState_Accepting && pSocketSlot->GetState() != SocketState_Normal)
            {
                WriteLog(LEVEL_DEBUG, "Send Data Failed. Slot=%d\n", pSocketSlot->GetSlotIndex());
                _Close(*pSocketSlot);
                return true;
            }
        }

    }
    return true;
}

bool CSocketMgr::SendMsg(MSG_BASE & rMsg, unsigned int nSlotIndex)
{
    CSocketSlot * pSocketSlot = m_SocketSlotMgr.GetSocketSlot(nSlotIndex);
    if (pSocketSlot == NULL)
    {
        WriteLog(LEVEL_ERROR, "CSocketMgr::SendMsg. Can't Find SocketSlot By Slot Index. nSlotIndex=%d.", nSlotIndex);
        return false;
    }
    else
    {
        if (pSocketSlot->SendMsg(rMsg))
        {
            if (!pSocketSlot->IsInSendQueue())
            {
                m_MutexForSendQueue.Lock();
                m_pAddSendQueue->push_back(pSocketSlot);
                m_MutexForSendQueue.Unlock();
                pSocketSlot->SetInSendQueue();
            }
            return true;
        }
        else
        {
            return false;
        }
    }
}

/*
   bool CSocketMgr::SendMsg(MSG_BASE & rMsg, unsigned int nSlotIndex)
   {
   CSocketSlot * pSocketSlot = m_SocketSlotMgr.GetSocketSlot(nSlotIndex);
   if (pSocketSlot == NULL)
   {
   WriteLog(LEVEL_ERROR, "CSocketMgr::SendMsg. Can't Find SocketSlot By Slot Index. nSlotIndex=%d.", nSlotIndex);
   return false;
   }
   else
   {
   bool bRemainData = false;
   bool bResult = pSocketSlot->SendMsg(rMsg, bRemainData);

// Remain some Data to be sent later
if (bRemainData)
{
_ModifyEvent(EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLET, pSocketSlot);
}
return bResult;
}
}
 */

void CSocketMgr::_Close(CSocketSlot & rSocketSlot)
{
    struct epoll_event ev;
    if (epoll_ctl(m_nEpollFd, EPOLL_CTL_DEL, rSocketSlot.GetFd(), &ev) < 0)
    {
        WriteLog(LEVEL_WARNING, "epoll delete failed: fd=%d.\n", rSocketSlot.GetFd());
    }

    if (!rSocketSlot.Close())
    {
        // the Disconnect msg can't put to the recvqueue
        m_SocketSlotMgr.ReleaseSlot(rSocketSlot);
    }
}

bool CSocketMgr::BroadcastMsg(MSG_BASE & rMsg)
{
    for (unsigned int i=0; i<m_nMaxClient; ++i)
    {
        SendMsg(rMsg, i);
    }
    return true;
}

void CSocketMgr::_CheckAlive()
{
    if (m_nAliveCheckInterval == 0)
    {
        return;
    }
    time_t tNow = time(NULL);
    if (tNow - m_tLastCheckTime >= (int)m_nAliveCheckInterval)
    {
        CSocketSlot * pSocketSlot = NULL;
        for (unsigned int i=0; i<m_nMaxClient; i++)
        {
            pSocketSlot = m_SocketSlotMgr.GetSocketSlot(i);
            if (pSocketSlot)
            {
                if (pSocketSlot->GetState() == SocketState_Normal || pSocketSlot->GetState() == SocketState_Accepting)
                {
                    if (tNow - pSocketSlot->GetLatestAliveTime() > (int)m_nAliveTimeOut)
                    {
                        pSocketSlot->SetStateNotAlive();
                        pSocketSlot = NULL;
                        continue;
                    }
                    pSocketSlot->SendAliveMsg();
                    if (!pSocketSlot->IsInSendQueue())
                    {
                        m_MutexForSendQueue.Lock();
                        m_pAddSendQueue->push_back(pSocketSlot);
                        m_MutexForSendQueue.Unlock();
                        pSocketSlot->SetInSendQueue();
                    }
                }

                pSocketSlot = NULL;
            }
        }
        m_tLastCheckTime = tNow;
    }
    else if (tNow < m_tLastCheckTime)
    {
        WriteLog(LEVEL_WARNING, "CSocketMgr::_CheckAlive. The clock turn back(Now=%u, LastTime=%u).", (unsigned int)tNow, (unsigned int)m_tLastCheckTime);
        m_tLastCheckTime = tNow;
    }
}

bool CSocketMgr::GetMsg(MSG_BASE * &pMsg, unsigned int & nSlotIndex)
{
    pMsg = NULL;
    /*
       if (m_bGetMsgThreadSafety)
       {
       m_MutextForGetMsg.Lock();
       }
     */
    while (true)
    {
        CRecvDataElement * pRecvData = NULL;
        if (!m_pRecvQueue->Pop(pRecvData))
        {
            _CheckAlive();
            break;
        }
        if (pRecvData == NULL)
        {
            WriteLog(LEVEL_ERROR, "CSocketMgr::GetMsg. Get NULL RecvData. There is something wrong with the recv queue.\n");
            continue;
        }
        pMsg = pRecvData->GetMsg();
        nSlotIndex = pRecvData->GetSlotIndex();
        if (pMsg != NULL)
        {
            _Pretreat(pMsg, nSlotIndex);
            if (pMsg != NULL)
            {
                break;
            }
        }
        else
        {
            WriteLog(LEVEL_ERROR, "CSocketMgr::GetMsg. The Msg of the RecvElement is NULL. SlotIndex = %d.\n", nSlotIndex);
        }
    }
    /*
       if (m_bGetMsgThreadSafety)
       {
       m_MutextForGetMsg.Unlock();
       }
     */
    return pMsg != NULL;
}

void CSocketMgr::_Pretreat(MSG_BASE * &pMsg, unsigned int & nSlotIndex)
{
    switch (pMsg->nMsg)
    {
        /*
           case MSGID_SYSTEM_SocketConnectSuccess:
           {
           delete pMsg;
           pMsg = NULL;
           }
           break;
           case MSGID_SYSTEM_SocketDisconnect:
           {
           delete pMsg;
           pMsg = NULL;
           }
           break;
         */
        case MSGID_SYSTEM_ConnectSuccess:
            {
                /*
                   CSocketSlot * pSocketSlot = m_SocketSlotMgr.GetSocketSlot(nSlotIndex);
                   if (pSocketSlot != NULL)
                   {
                   pSocketSlot->SetStateConnected();
                   }
                   else
                   {
                   WriteLog(LEVEL_ERROR, "Disconnect. Can't find the slot. MsgID=%d, SlotIndex = %d.\n", MSGID_SYSTEM_ConnectSuccess, pSocketSlot->GetSlotIndex());
                   }
                 */
            }
            break;
        case MSGID_SYSTEM_Disconnect:
            {
                CSocketSlot * pSocketSlot = m_SocketSlotMgr.GetSocketSlot(nSlotIndex);
                if (pSocketSlot && !pSocketSlot->IsConnectSuccess())
                {
                    delete pMsg;
                    pMsg = NULL;
                }
                m_SocketSlotMgr.ReleaseSlot(nSlotIndex);
            }
            break;
        default:
            break;
    }
}

void CSocketMgr::DisconnectClient(unsigned int nSlotIndex, int nDisconnectReason)
{
    CSocketSlot * pSocketSlot = m_SocketSlotMgr.GetSocketSlot(nSlotIndex);
    if (pSocketSlot != NULL)
    {
        if (pSocketSlot->GetState() == SocketState_Normal)
        {
            pSocketSlot->Disconnect(nDisconnectReason);
        }
    }
    else
    {
        WriteLog(LEVEL_ERROR, "CSocketMgr::DisconnectClient. Can't find the slot. SlotIndex = %d.\n", pSocketSlot->GetSlotIndex());
    }
}

void CSocketMgr::DisconnectAllClient(int nDisconnectReason)
{
    for (unsigned int i=0; i<m_nMaxClient; ++i)
    {
        DisconnectClient(i, nDisconnectReason);
    }
}

void CSocketMgr::Stop()
{
    m_bRunning = false;
}

