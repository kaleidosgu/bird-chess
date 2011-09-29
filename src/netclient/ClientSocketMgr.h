#ifndef __clinetnet_clientsocketmgr_h_
#define __clinetnet_clientsocketmgr_h_

#include "../net/msgbasic.h"
#include <WinSock2.h>
#include <string>
#include <list>

#ifndef UM_MSG
#define UM_MSG (WM_USER + 1023)
#endif // UM_MSG

using namespace std;

namespace clinetnet
{
	enum SocketState
	{
		SocketState_Free,
		SocketState_Connecting,
		SocketState_Normal,
		SocketState_Closed
	};
    class CClientSocketMgr
    {
    public:
        CClientSocketMgr();
        ~CClientSocketMgr();
        bool Init(HWND hWndNotify = NULL);
        bool Connect(string strIP, unsigned short nPort);
        bool Reconnect();
        MSG_BASE * GetMsg();
        bool SendMsg(const MSG_BASE & rMsg, bool bToSelf = false);
        void Disconnect();
        bool IsConnected();

    private:
		static DWORD WINAPI SendThreadProc(LPVOID pParam);
		static DWORD WINAPI RecvThreadProc(LPVOID pParam);
		void ProcessSend();
		void ProcessRecv();
		bool _Send();
		bool _Recv();
		bool _DisposeMsgData();
		void _Pretreat(MSG_BASE * &pMsg);
		void _AddSendMsg(MSG_BASE * pMsg);
		void _AddRecvMsg(const MSG_BASE & rMsg);
		MSG_BASE * _GetSendMsg();

		HWND m_hWndNotify;
        string m_strIP;
        unsigned short m_nPort;
        int m_hSocket;

		SocketState m_eSocketState;
		
		WSAEVENT m_hConnectEvent;
		WSAEVENT m_hSocketEvent;
		
        // for recv
        char m_Buffer[cMAX_PACKET_SIZE];
        unsigned short m_nBytesRemain;

		HANDLE m_hSendEvent;
		list< MSG_BASE * > m_SendMsgList;
		CRITICAL_SECTION m_SendMsgListCS;

		list< MSG_BASE * > m_RecvMsgList;
		CRITICAL_SECTION m_RecvMsgListCS;

		HANDLE m_hSendThread;
		DWORD m_nSendThreadID;
		HANDLE m_hRecvThread;
		DWORD m_nRecvThreadID;
    };

}

#endif // __clinetnet_clientsocketmgr_h_

