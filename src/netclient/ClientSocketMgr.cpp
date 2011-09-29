//#include <errno.h>

#include "ClientSocketMgr.h"
#include "../net/systemmsg.h"
#include <time.h>

using namespace clinetnet;

CClientSocketMgr::CClientSocketMgr()
{
	m_hWndNotify = NULL;
    m_strIP = "";
    m_nPort = 0;
	m_hSocket = INVALID_SOCKET;

	m_eSocketState = SocketState_Free;

	m_hConnectEvent = WSA_INVALID_EVENT;
	m_hSocketEvent = WSA_INVALID_EVENT;
	
    memset(m_Buffer, 0, cMAX_PACKET_SIZE);
    m_nBytesRemain = 0;

	m_hSendEvent = NULL;
	InitializeCriticalSection(&m_SendMsgListCS);
	InitializeCriticalSection(&m_RecvMsgListCS);

	m_hSendThread = NULL;
	m_nSendThreadID = 0;
	m_hRecvThread = NULL;
	m_nRecvThreadID = 0;
}

CClientSocketMgr::~CClientSocketMgr()
{
	Disconnect();
	WSACloseEvent(m_hSocketEvent);
	WSACloseEvent(m_hConnectEvent);
	DeleteCriticalSection(&m_RecvMsgListCS);
	DeleteCriticalSection(&m_SendMsgListCS);
	if (m_hSendEvent)
	{
		CloseHandle(m_hSendEvent);
		m_hSendEvent = NULL;
	}
	while (!m_SendMsgList.empty())
	{
		delete m_SendMsgList.front();
		m_SendMsgList.pop_front();
	}
	while (!m_RecvMsgList.empty())
	{
		delete m_RecvMsgList.front();
		m_RecvMsgList.pop_front();
	}
	m_hWndNotify = NULL;
}


void CClientSocketMgr::_AddSendMsg(MSG_BASE * pMsg)
{
	EnterCriticalSection(&m_SendMsgListCS);
	m_SendMsgList.push_back(pMsg);
	LeaveCriticalSection(&m_SendMsgListCS);
	SetEvent(m_hSendEvent);
}

void CClientSocketMgr::_AddRecvMsg(const MSG_BASE & rMsg)
{
	MSG_BASE * pMsg = CreateDynamicLengthMsg(rMsg.nSize, (MSG_BASE *)0);
	memcpy(pMsg, &rMsg, rMsg.nSize);
	EnterCriticalSection(&m_RecvMsgListCS);
	m_RecvMsgList.push_back(pMsg);
	LeaveCriticalSection(&m_RecvMsgListCS);
	if (m_hWndNotify)
	{
		PostMessage(m_hWndNotify, UM_MSG, pMsg->nMsg, pMsg->nSize);
	}
}

bool CClientSocketMgr::Init(HWND hWndNotify/* = NULL*/)
{
	m_hWndNotify = hWndNotify;
	WSADATA WSAData;
	if (WSAStartup(WINSOCK_VERSION, &WSAData))
	{
		return false;
	}
	//ASSERT(m_hEventConnect == WSA_INVALID_EVENT);
	m_hSocketEvent = WSACreateEvent();
	TCHAR szName[MAX_PATH];
	sprintf_s(szName, "SendEvent%d%d", this, (int)time(NULL));
	m_hSendEvent = CreateEvent(NULL, FALSE, FALSE, szName);
	return true;
}

bool CClientSocketMgr::Connect(string strIP, unsigned short nPort)
{
	if (m_eSocketState != SocketState_Free)
	{
		return false;
	}
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

bool CClientSocketMgr::Reconnect()
{
    if (m_nPort == 0 || m_strIP == "")
    {
        //WriteLog(LEVEL_ERROR,"CClientSocketMgr::Connect. You may have not initialized the clientsocketmgr.\n");
        return false;
    }
	if (m_eSocketState != SocketState_Free)
	{
		return false;
	}
	m_eSocketState = SocketState_Connecting;
	if (m_hSocket != INVALID_SOCKET)
	{
		closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET;
	}
	
	// reset recv buffer
	memset(m_Buffer, 0, cMAX_PACKET_SIZE);
    m_nBytesRemain = 0;

    m_hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (m_hSocket == INVALID_SOCKET)
    {
        //WriteLog(LEVEL_ERROR,"CClientSocketMgr::Connect. socket() error.\n");
		m_eSocketState = SocketState_Free;
        return false;
    }
    sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr.s_addr = inet_addr(m_strIP.c_str());
    sockAddr.sin_port = htons(m_nPort);

	if (m_hConnectEvent != WSA_INVALID_EVENT)
	{
		WSACloseEvent(m_hConnectEvent);
	}
	m_hConnectEvent = WSACreateEvent();
	int nErrorCode = WSAEventSelect(m_hSocket, m_hConnectEvent, FD_CONNECT);
	if (nErrorCode == SOCKET_ERROR)
	{
		//TRACE(_T("__ClientSocketMgr WSAEventSelect connect failed.\n"));
		closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET;
		m_eSocketState = SocketState_Free;
		return false;
	}

    int nConnect = connect(m_hSocket, (sockaddr*)&sockAddr, sizeof(sockAddr));
    if (nConnect == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK)
    {
		//TRACE(_T("__ClientSocketMgr Socket connect error.\n"));
        closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET;
		m_eSocketState = SocketState_Free;
        return false;
    }
    else
    {
		const DWORD cCONNECT_TIMEOUT = 5000;
		DWORD dwWaitRes = WaitForSingleObject(m_hConnectEvent, cCONNECT_TIMEOUT);
		if (dwWaitRes != WAIT_OBJECT_0)
		{
			// log connect failed
			closesocket(m_hSocket);
			m_hSocket = INVALID_SOCKET;
			m_eSocketState = SocketState_Free;
			return false;
		}

		WSANETWORKEVENTS events;
		if (0 == WSAEnumNetworkEvents(m_hSocket, m_hConnectEvent, &events))
		{
			if (events.lNetworkEvents & FD_CONNECT)
			{
				if ( 0 != events.iErrorCode[FD_CONNECT_BIT])
				{
					closesocket(m_hSocket);
					m_hSocket = INVALID_SOCKET;
					m_eSocketState = SocketState_Free;
					return false;
				}
			}
		}

		nErrorCode = WSAEventSelect(m_hSocket, m_hSocketEvent, FD_READ | FD_CLOSE);
		if (nErrorCode == SOCKET_ERROR)
		{
			// log
			closesocket(m_hSocket);
			m_hSocket = INVALID_SOCKET;
			m_eSocketState = SocketState_Free;
			return false;
		}

		m_hSendThread = CreateThread(NULL, 0, SendThreadProc, this, 0, &m_nSendThreadID);
		if (m_hSendThread == NULL)
		{
			//TRACE(_T("__ClientSocketMgr Create Send thread failed.\n"));
			Disconnect();
			return false;
		}
		m_hRecvThread = CreateThread(NULL, 0, RecvThreadProc, this, 0, &m_nRecvThreadID);
		if (m_hRecvThread == NULL)
		{
			//TRACE(_T("__ClientSocketMgr Create Recv thread failed.\n"));
			Disconnect();
			return false;
		}
		
		MSG_SYSTEM_ConnectSuccess msg;
		_AddRecvMsg(msg);

		m_eSocketState = SocketState_Normal;

		int nOptVal = 1;
		setsockopt(m_hSocket, IPPROTO_TCP, TCP_NODELAY, (char *)&nOptVal, sizeof(int));

		return true;
    }
}

bool CClientSocketMgr::IsConnected()
{
	return m_eSocketState == SocketState_Normal;
}

MSG_BASE * CClientSocketMgr::GetMsg()
{
    MSG_BASE * pMsg = NULL;
	while (true)
	{
		EnterCriticalSection(&m_RecvMsgListCS);
		if (!m_RecvMsgList.empty())
		{
			pMsg = m_RecvMsgList.front();
			m_RecvMsgList.pop_front();
		}
		LeaveCriticalSection(&m_RecvMsgListCS);

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
			break;
		}
	}
	if (pMsg == NULL)
	{
		// Check Ping ?
	}
    return pMsg;
}

void CClientSocketMgr::_Pretreat(MSG_BASE * &pMsg)
{
    switch (pMsg->nMsg)
    {
        case MSGID_SYSTEM_ConnectSuccess:
            {
				//m_eSocketState = SocketState_Normal;
            }
            break;
        case MSGID_SYSTEM_Disconnect:
            {
				Disconnect();
            }
            break;
        default:
            break;
    }
}

bool CClientSocketMgr::SendMsg(const MSG_BASE & rMsg, bool bToSelf/* = false */)
{
	if (m_eSocketState != SocketState_Normal)
	{
		// log the state is not normal
		return false;
	}
	if (rMsg.nMsg < sizeof(MSG_BASE) && rMsg.nSize > cMAX_PACKET_SIZE)
	{
		// log msg's size is error.
		return false;
	}

	if (bToSelf)
	{
		_AddRecvMsg(rMsg);
	}
	else
	{
		// if the msg is system msg, need dispose with it?
		// need compress and encrypt ?
		MSG_BASE * pMsg = CreateDynamicLengthMsg(rMsg.nSize, (MSG_BASE *)0);
		memcpy(pMsg, &rMsg, rMsg.nSize);
		_AddSendMsg(pMsg);
	}
    return true;
}

MSG_BASE * CClientSocketMgr::_GetSendMsg()
{
	MSG_BASE * pMsg = NULL;
	EnterCriticalSection(&m_SendMsgListCS);
	while (!m_SendMsgList.empty())
	{
		pMsg = m_SendMsgList.front();
		m_SendMsgList.pop_front();
	}
	LeaveCriticalSection(&m_SendMsgListCS);
	return pMsg;
}

void CClientSocketMgr::Disconnect()
{
	m_eSocketState = SocketState_Closed;

	if (m_hSocket != INVALID_SOCKET)
	{
		closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET;
	}
	if (m_hSendThread)
	{
		WaitForSingleObject(m_hSendThread, INFINITE);
		CloseHandle(m_hSendThread);
		m_hSendThread = NULL;
		m_nSendThreadID = 0;
	}
	if (m_hRecvThread)
	{
		WaitForSingleObject(m_hRecvThread, INFINITE);
		CloseHandle(m_hRecvThread);
		m_hRecvThread = NULL;
		m_nRecvThreadID = 0;
	}
	list< MSG_BASE * > tmpRecvMsgList;
	EnterCriticalSection(&m_RecvMsgListCS);
	if (!m_RecvMsgList.empty())
	{
		tmpRecvMsgList = m_RecvMsgList;
		m_RecvMsgList.clear();
	}
	LeaveCriticalSection(&m_RecvMsgListCS);
	
	while (!tmpRecvMsgList.empty())
	{
		delete tmpRecvMsgList.front();
		tmpRecvMsgList.pop_front();
	}

	EnterCriticalSection(&m_SendMsgListCS);
	while (!m_SendMsgList.empty())
	{
		delete m_SendMsgList.front();
		m_SendMsgList.pop_front();
	}
	LeaveCriticalSection(&m_SendMsgListCS);
	m_eSocketState = SocketState_Free;
}

DWORD CClientSocketMgr::SendThreadProc(LPVOID pParam)
{
	CClientSocketMgr * pCSM = (CClientSocketMgr *)pParam;
	pCSM->ProcessSend();
	return 0;
}
DWORD CClientSocketMgr::RecvThreadProc(LPVOID pParam)
{
	CClientSocketMgr * pCSM = (CClientSocketMgr *)pParam;
	pCSM->ProcessRecv();
	return 0;
}

void CClientSocketMgr::ProcessSend()
{
	while (m_eSocketState != SocketState_Closed)
	{
		DWORD dwWaitRes = WaitForSingleObject(m_hSendEvent, 20);
		if (dwWaitRes == WAIT_OBJECT_0)
		{
			if (!_Send())
			{
				m_eSocketState = SocketState_Closed;
			}
		}
	}
	MSG_SYSTEM_Disconnect msg;
	_AddRecvMsg(msg);
}

void CClientSocketMgr::ProcessRecv()
{
	WSANETWORKEVENTS events;
	while (m_eSocketState != SocketState_Closed)
	{
		DWORD dwWaitRes = WaitForSingleObject(m_hSocketEvent, 20);
		if (dwWaitRes == WAIT_OBJECT_0)
		{
			if (0 == WSAEnumNetworkEvents(m_hSocket, m_hSocketEvent, &events))
			{
				if (events.lNetworkEvents & FD_READ)
				{
					if (!_Recv())
					{
						m_eSocketState = SocketState_Closed;
					}
				}
				if (events.lNetworkEvents & FD_CLOSE)
				{
					int nErrorCode = events.iErrorCode[FD_CLOSE_BIT];
					switch (nErrorCode)
					{
					case WSAENETDOWN:
						// log
						break;
					case WSAECONNRESET:
						// log
						break;
					case WSAECONNABORTED:
						// log
						break;
					}
					// log socket closed[%d] nErrorCode
					m_eSocketState = SocketState_Closed;
				}
			}
		}
	}
}

bool CClientSocketMgr::_Send()
{
	MSG_BASE * pMsg = NULL;
	while ((pMsg = _GetSendMsg()) != NULL)
	{
		char * pData = (char *)pMsg;
		int nBytes = 0;
		while (nBytes < pMsg->nSize)
		{
			int nLen = send(m_hSocket, pData + nBytes, pMsg->nSize - nBytes, 0);
			if (nLen != SOCKET_ERROR)
			{
				nBytes += nLen;
			}
			else if (WSAGetLastError() == WSAEWOULDBLOCK)
			{
				Sleep(10);
			}
			else
			{
				delete pMsg;
				pMsg = NULL;
				return false;
			}
		}

		delete pMsg;
		pMsg = NULL;
	}
	return true;
}

bool CClientSocketMgr::_Recv()
{
    //if (m_eSocketState != SocketState_Normal)
    //{
    //    //WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::RevData. The socket state is not normal. State = %d.\n", m_nSlotIndex, m_State);
    //    return false;
    //}

    bool bRes = true;

    int nLen = 0;
    while (true)
    {
        nLen = recv(m_hSocket, m_Buffer + m_nBytesRemain, cMAX_PACKET_SIZE - m_nBytesRemain, 0);
        if (nLen > 0)
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
        else if (nLen == 0)
        {
            //_SetState(m_State == SocketState_Accepting ? SocketState_Accepting : SocketState_Normal, SocketState_Broken, DisconnectReason_Shutdown);
			bRes = false;
            break;
        }
        else
        {
            int nErr = errno;
            if (WSAGetLastError() != WSAEWOULDBLOCK)
            {
                //_SetState(m_State == SocketState_Accepting ? SocketState_Accepting : SocketState_Normal, SocketState_Broken, DisconnectReason_ReadError);
                //WriteLog(LEVEL_DEBUG, "CSocketSlot(%d)::RecvData. Read error. ErrInfo=(%s); m_nBytesRemain = %d.\n", m_nSlotIndex, strerror(nErr), m_nBytesRemain);
                bRes = false;
            }
            break;
        }

    }
    return bRes;
}

bool CClientSocketMgr::_DisposeMsgData()
{
    //if (m_eSocketState != SocketState_Normal)
    //{
    //    //WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_DisposeMsgData. The socket state is not normal. State = %d.\n", m_nSlotIndex, m_State);
    //    return false;
    //}

    bool bRes = true;
    char * pPos = m_Buffer;

    while (m_nBytesRemain >= sizeof(MSG_BASE))
    {
        MSG_BASE *pMsg = (MSG_BASE *)pPos;

        // if Size is Larger than Max Size, Error
        if (pMsg->nSize > cMAX_PACKET_SIZE)
        {
            //_SetState(m_State == SocketState_Accepting ? SocketState_Accepting : SocketState_Normal, SocketState_Broken, DisconnectReason_ErrMsgSize);
            //WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_DisposeMsgData ErrMsgSize: too large. Size(%d), MaxSize(%d).\n", m_nSlotIndex, pMsg->nSize, cMAX_PACKET_SIZE);
            bRes = false;
            break;

        }

        if (m_nBytesRemain < pMsg->nSize)
        {
            break;
        }

        if (pMsg->nSize < sizeof(MSG_BASE))
        {
            //_SetState(m_State == SocketState_Accepting ? SocketState_Accepting : SocketState_Normal, SocketState_Broken, DisconnectReason_ErrMsgSize);
            //WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_DisposeMsgData ErrMsgSize: too small. Size(%d), MinSize(%d).\n", m_nSlotIndex, pMsg->nSize, sizeof(MSG_BASE));
            bRes = false;
            break;
        }
		_AddRecvMsg(*pMsg);
		m_nBytesRemain -= pMsg->nSize;
		pPos += pMsg->nSize;
    }
    if (m_nBytesRemain > 0 && pPos != m_Buffer)
    {
        memmove(m_Buffer, pPos, m_nBytesRemain);
    }
    return bRes;
}
