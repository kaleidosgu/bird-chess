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

    // TODO : Generate public key
    MSG_SYSTEM_ClientPublicKey * pCPKMsg = new MSG_SYSTEM_ClientPublicKey();
    if (!_SendMsgDirectly(pCPKMsg))
    {
        WriteLog(LEVEL_WARNING, "CClientSocketSlot::OnConnect. Send client public key failed.\n");
    }

    _GenerateSecretKey();
    // TODO : Encrypt

    MSG_SYSTEM_C2S_SecretKey * pSKMsg = new MSG_SYSTEM_C2S_SecretKey();
    if (!_SendMsgDirectly(pSKMsg))
    {
        WriteLog(LEVEL_WARNING, "CClientSocketSlot::OnConnect. Send secret key failed.\n");
    }

    // Add SocketConnectSuccessMsg to Recv queue
    MSG_SYSTEM_SocketConnectSuccess * pSCSMsg = new MSG_SYSTEM_SocketConnectSuccess();
    if (!_AddRecvMsg(pSCSMsg))
    {
        delete pSCSMsg;
        pSCSMsg = NULL;
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
bool CClientSocketSlot::_DisposeRecvMsg(MSG_BASE & rMsg)
{
    bool bRes = true;
    switch (rMsg.nMsg)
    {
        case MSGID_SYSTEM_S2C_SecretKey:
            {
                MSG_SYSTEM_S2C_SecretKey & rSKMsg = (MSG_SYSTEM_S2C_SecretKey &)rMsg;
                // TODO : Decrypt
                // m_DecryptKey = "";

                _SetState(SocketState_Accepting, SocketState_Normal);

                MSG_SYSTEM_ConnectSuccess * pConnectSuccessMsg = new MSG_SYSTEM_ConnectSuccess();
                if (!_AddRecvMsg(pConnectSuccessMsg))
                {
                    delete pConnectSuccessMsg;
                    pConnectSuccessMsg = NULL;
                    WriteLog(LEVEL_ERROR, "CClientSocketSlot::OnConnect. Add connect success msg failed.\n");
                    bRes = false;
                }
            }
            break;
        case MSGID_SYSTEM_S2C_UpdateSecretKey:
            {
                // TODO : Decrypt
                // m_DecryptKey = "";
            }
            break;
        default:
            bRes = CSocketSlot::_DisposeRecvMsg(rMsg);
            break;
    }
    return bRes;
}
