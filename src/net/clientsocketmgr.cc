#include "clientsocketmgr.h"
#include "recvdataelement.h"
#include "../base/log.h"
#include "systemmsg.h"
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

const int cMAX_EPOLL_QUEUE_FOR_CLIENT = 16;

const unsigned int cRECV_QUEUE_SIZE = 65536;


using namespace net;
using namespace base;

CClientSocketMgr::CClientSocketMgr()
{
    m_bGetMsgThreadSafety = false;
    m_pRecvQueue = new LoopQueue< CRecvDataElement * >(cRECV_QUEUE_SIZE);
    m_ClientSocketSlot.SetRecvQueue(m_pRecvQueue);

    // create epoll
    m_nEpollFd = epoll_create(cMAX_EPOLL_QUEUE_FOR_CLIENT);

    m_strIP = "";
    m_nPort = 0;
    m_nFd = -1;

}

CClientSocketMgr::~CClientSocketMgr()
{
}

bool CClientSocketMgr::_ModifyEvent(unsigned int nEvents, int nFd)
{
    struct epoll_event ev;
    ev.events = nEvents;
    ev.data.fd = nFd;
    if (epoll_ctl(m_nEpollFd, EPOLL_CTL_MOD, nFd, &ev) < 0)
    {
        WriteLog(LEVEL_ERROR, "epoll set modification failed: fd=%d.\n", m_ClientSocketSlot.GetFd());
        return false;
    }
    return true;
}

bool CClientSocketMgr::_AddEvent(int nFd, unsigned int nEvents)
{
    struct epoll_event ev;
    ev.events = nEvents;
    ev.data.fd = nFd;
    if (epoll_ctl(m_nEpollFd, EPOLL_CTL_ADD, nFd, &ev) < 0)
    {
        WriteLog(LEVEL_ERROR, "epoll add event failed: fd=%d.\n", nFd);
        return false;
    }
    return true;
}

bool CClientSocketMgr::Init(bool bGetMsgThreadSafety)
{
    bool bRes = true;
    m_bGetMsgThreadSafety = bGetMsgThreadSafety;
    return bRes;
}

bool CClientSocketMgr::_InitClientSocketMgr()
{
    bool bRes = true;
    return bRes;
}

bool CClientSocketMgr::Reconnect()
{
    if (m_nPort == 0 || m_strIP == "")
    {
        WriteLog(LEVEL_ERROR,"CClientSocketMgr::Connect. You may have not initialized the clientsocketmgr.\n");
        return false;
    }
    bool bRes = true;
    sockaddr_in sockAddr;
    bzero((char *)&sockAddr, sizeof(sockAddr));
    m_nFd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (m_nFd == -1)
    {
        WriteLog(LEVEL_ERROR,"CClientSocketMgr::Connect. socket() error.\n");
        return false;
    }
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr.s_addr = inet_addr(m_strIP.c_str());
    sockAddr.sin_port = htons(m_nPort);
    int nConnect = connect(m_nFd, (sockaddr*)&sockAddr, sizeof(sockAddr));
    if (nConnect < 0)
    {
        int nErr = errno;
        WriteLog(LEVEL_ERROR,"CClientSocketMgr::Connect. connect() error(%d).\n", nErr);
        close(m_nFd);
        return false;
    }
    else
    {
        if (!_AddEvent(m_nFd, EPOLLIN | EPOLLRDHUP | EPOLLET))
        {
            WriteLog(LEVEL_ERROR, "epoll add event failed. fd=%d.\n", m_nFd);
            close(m_nFd);
            return false;
        }
    }
    m_ClientSocketSlot.OnConnect(m_nFd, sockAddr);

    return bRes;
}

bool CClientSocketMgr::Connect(string strIP, unsigned short nPort)
{
    if (strIP == "")
    {
        return false;
    }
    if (nPort == 0)
    {
        return false;
    }
    m_strIP = strIP;
    m_nPort = nPort;
    return Reconnect();
}

bool CClientSocketMgr::IsDisconnected()
{
    return (m_ClientSocketSlot.GetState() == SocketState_Free);
}

MSG_BASE * CClientSocketMgr::GetMsg()
{
    MSG_BASE *pMsg = NULL;
    if (m_bGetMsgThreadSafety)
    {
        m_MutextForGetMsg.Lock();
    }
    while (true)
    {
        CRecvDataElement * pRecvData = NULL;
        if (!m_pRecvQueue->Pop(pRecvData))
        {
            break;
        }
        if (pRecvData == NULL)
        {
            WriteLog(LEVEL_ERROR, "CClientSocketMgr::GetMsg. Get NULL RecvData. There is something wrong with the recv queue.\n");
            continue;
        }
        pMsg = pRecvData->GetMsg();
        if (pMsg != NULL)
        {
            _Pretreat(pMsg);
            if (pMsg != NULL)
            {
                break;
            }
        }
        else
        {
            WriteLog(LEVEL_ERROR, "CClientSocketMgr::GetMsg. The Msg of the RecvElement is NULL.\n");
        }
    }
    if (m_bGetMsgThreadSafety)
    {
        m_MutextForGetMsg.Unlock();
    }
    return pMsg;
}

void CClientSocketMgr::_Pretreat(MSG_BASE * &pMsg)
{
    switch (pMsg->nMsg)
    {
        /*
        case MSGID_SYSTEM_ConnectSuccess:
            {
                m_ClientSocketSlot.SetStateConnected();
            }
            break;
            */
        case MSGID_SYSTEM_Disconnect:
            {
                m_ClientSocketSlot.Reset();
            }
            break;
        case MSGID_SYSTEM_CheckAlive:
            {
                MSG_SYSTEM_CheckAliveReply checkAliveReplyMsg;
                SendMsg(checkAliveReplyMsg);
            }
            break;
        default:
            break;
    }
}

bool CClientSocketMgr::SendMsg(MSG_BASE & rMsg)
{
    return m_ClientSocketSlot.SendMsg(rMsg);
}

void CClientSocketMgr::Disconnect()
{
    if (m_ClientSocketSlot.GetState() == SocketState_Normal)
    {
        m_ClientSocketSlot.Disconnect(DisconnectReason_Unkown);
        _ModifyEvent(EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLET, m_ClientSocketSlot.GetFd());
    }
    else
    {
        m_bRunning = false;
    }
}

void CClientSocketMgr::Process()
{
    if (!_InitClientSocketMgr())
    {
        WriteLog(LEVEL_ERROR, "Init Socket Server Failed.\n");
        return;
    }

    m_bRunning = true;

    int nfds = 0;
    struct epoll_event * events = new epoll_event[cMAX_EPOLL_QUEUE_FOR_CLIENT];
    bool bBusy = false;
    while (m_bRunning)
    {
        bBusy = false;

        // recv
        nfds = epoll_wait(m_nEpollFd, events, cMAX_EPOLL_QUEUE_FOR_CLIENT, 0);
        if (nfds > 0)
        {
            bBusy = true;
        }
        for(int i = 0; i < nfds; ++i)
        {
            if(events[i].data.fd == m_nFd)
            {
                _ProcessEpollEvent(events[i].events);
            }
        }

        // send
        if (m_ClientSocketSlot.NeedSendData())
        {
            bBusy = true;
            if (m_ClientSocketSlot.SendData())
            {
                if (m_ClientSocketSlot.GetState() == SocketState_Accepting || m_ClientSocketSlot.GetState() == SocketState_Normal)
                {
                    _ModifyEvent(EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLET, m_ClientSocketSlot.GetFd());
                }
            }
        }

        // if not busy, then sleep for a moment.
        if (!bBusy)
        {
            usleep(1000);
        }
    }
    WriteLog("Process Complete.\n");
}

bool CClientSocketMgr::_ProcessEpollEvent(unsigned int nEvents)
{
    if (nEvents & (EPOLLERR))
    {
        //WriteLog(LEVEL_DEBUG, "Received EPOLLERR event. Slot=%d\n", m_ClientSocketSlot.GetSlotIndex());
        _Close();
        return true;
    }
    if (nEvents & EPOLLHUP)
    {
        //WriteLog(LEVEL_DEBUG, "Received EPOLLHUP event. Slot=%d\n", m_ClientSocketSlot.GetSlotIndex());
        _Close();
        return true;
    }
    if (nEvents & EPOLLRDHUP)
    {
        //WriteLog(LEVEL_DEBUG, "Received EPOLLRDHUP event. Slot=%d\n", m_ClientSocketSlot.GetSlotIndex());
        _Close();
        return true;
    }
    if (nEvents & EPOLLIN)
    {
        //WriteLog(LEVEL_DEBUG, "Received EPOLLIN event.\n");
        if (!m_ClientSocketSlot.RecvData())
        {
            WriteLog(LEVEL_DEBUG, "Receive Data Failed.\n");
            _Close();
            return true;
        }
        /*
        if (m_ClientSocketSlot.GetState() == SocketState_Broken)
        {
            WriteLog(LEVEL_DEBUG, "Send Data Failed.\n");
            _Close();
            return true;
        }
        else if (m_ClientSocketSlot.GetState() != SocketState_Accepting && m_ClientSocketSlot.GetState() != SocketState_Normal)
        {
            WriteLog(LEVEL_WARNING, "What's wrong with the State. State = %d.\n", m_ClientSocketSlot.GetState());
        }
        */
    }
    if (nEvents & EPOLLOUT)
    {
        //WriteLog(LEVEL_DEBUG, "Received EPOLLOUT event.\n");
        if (m_ClientSocketSlot.GetState() == SocketState_Disconnecting)
        {
            WriteLog(LEVEL_DEBUG, "The socket's state is disconnecting.\n");
            _Close();
            m_bRunning = false;
            return true;
        }

        if (m_ClientSocketSlot.SendData())
        {
            _ModifyEvent(EPOLLIN | EPOLLRDHUP | EPOLLET, m_ClientSocketSlot.GetFd());
        }
        else
        {
            // 1. system buffer is full
            // 2. send error

            // if send error, close
            if (m_ClientSocketSlot.GetState() != SocketState_Normal)
            {
                WriteLog(LEVEL_DEBUG, "Send Data Failed. Slot=%d\n", m_ClientSocketSlot.GetSlotIndex());
                _Close();
                return true;
            }
        }
    }
    return true;
}

void CClientSocketMgr::_Close()
{
    struct epoll_event ev;
    if (epoll_ctl(m_nEpollFd, EPOLL_CTL_DEL, m_ClientSocketSlot.GetFd(), &ev) < 0)
    {
        WriteLog(LEVEL_WARNING, "epoll delete failed: fd=%d.\n", m_ClientSocketSlot.GetFd());
    }

    if (!m_ClientSocketSlot.Close())
    {
        m_ClientSocketSlot.Reset();
    }
}

