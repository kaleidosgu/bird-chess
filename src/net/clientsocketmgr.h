#ifndef __net_clientsocketmgr_h_
#define __net_clientsocketmgr_h_

#include "msgbasic.h"
#include "clientsocketslot.h"
#include "socketmgr.h"

using namespace base;

namespace net
{
    class CClientSocketMgr : public Thread
    {
    public:
        CClientSocketMgr();
        ~CClientSocketMgr();
        bool Init(bool bGetMsgThreadSafety = false);
        bool Connect(string strIP, unsigned short nPort);
        bool Reconnect();
        MSG_BASE * GetMsg();
        bool SendMsg(MSG_BASE & rMsg);
        void Disconnect();
        bool IsDisconnected();
    private:
        virtual void Process();
        bool _ModifyEvent(unsigned int nEvents, int nFd);
        bool _AddEvent(int nFd, unsigned int nEvents);

        bool _InitClientSocketMgr();
        void _Pretreat(MSG_BASE * &pMsg);
        bool _ProcessEpollEvent(unsigned int nEvents);
        void _Close();

        int m_nFd;
        CClientSocketSlot m_ClientSocketSlot;
        int m_nEpollFd;
        string m_strIP;
        unsigned short m_nPort;
        //CRecvDataElementArray m_RecvArray;
        //CRecvDataElementQueue m_RecvQueue;
        LoopQueue< CRecvDataElement * > * m_pRecvQueue;
        CSocketSlotMgr m_SocketSlotMgr;

        bool m_bGetMsgThreadSafety;
        Mutex m_MutextForGetMsg;

        bool m_bRunning;
    };

}

#endif // __net_clientsocketmgr_h_
