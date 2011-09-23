#include "clientsocketslot.h"
#include "systemmsg.h"
#include <sys/socket.h>

using namespace net;

CClientSocketSlot::CClientSocketSlot()
{
}

CClientSocketSlot::~CClientSocketSlot()
{
}

void CClientSocketSlot::OnConnect(int nFd, const sockaddr_in &rSockAddr)
{
    m_nFd = nFd;
    m_addr = rSockAddr.sin_addr;
    m_nPort = ntohs(rSockAddr.sin_port);

    SetNonBlocking(m_nFd);

    _SetState(SocketState_Free, SocketState_Normal);
    MSG_SYSTEM_ConnectSuccess connectSuccessMsg;
    _AddRecvMsg(connectSuccessMsg);
}

void CClientSocketSlot::SetRecvQueue(LoopQueue< CRecvDataElement * > * pRecvQueue)
{
    _SetRecvQueue(pRecvQueue);
}

void CClientSocketSlot::Reset()
{
    _Reset();
}

