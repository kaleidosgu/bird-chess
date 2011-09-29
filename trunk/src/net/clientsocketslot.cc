#include "clientsocketslot.h"
#include "systemmsg.h"
#include <sys/socket.h>
#include "../base/log.h"

using namespace net;
using namespace base;

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

    _SetState(SocketState_Free, SocketState_Accepting);
    MSG_SYSTEM_ConnectSuccess * pConnectSuccessMsg = new MSG_SYSTEM_ConnectSuccess();
    if (!_AddRecvMsg(pConnectSuccessMsg))
    {
        delete pConnectSuccessMsg;
        pConnectSuccessMsg = NULL;
        WriteLog(LEVEL_ERROR, "CClientSocketSlot::OnConnect. Add connect success msg failed.\n");
    }
}

void CClientSocketSlot::SetRecvQueue(LoopQueue< CRecvDataElement * > * pRecvQueue)
{
    _SetRecvQueue(pRecvQueue);
}

void CClientSocketSlot::Reset()
{
    _Reset();
}

bool CClientSocketSlot::NeedSendData()
{
    if (m_pMsgCache != NULL)
    {
        return true;
    }
    if (!m_SendQueue.IsEmpty())
    {
        return true;
    }
    if (m_pSendDataHead != m_pSendDataTail)
    {
        return true;
    }
    return false;
}
