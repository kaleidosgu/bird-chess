#ifndef __net_socketmgr_h_
#define __net_socketmgr_h_

#include "../base/thread.h"
#include "socketslot.h"
#include "recvdataelementqueue.h"
#include "recvdataelementarray.h"
#include <sys/epoll.h>

using namespace base;

namespace net
{
    class CSocketMgr : public Thread
    {
    public:
        CSocketMgr();
        ~CSocketMgr();
        bool Init(unsigned short nPort, unsigned int nMaxClient, bool bGetMsgThreadSafety = false);

        bool SendMsg(MSG_BASE & rMsg, unsigned int nSlotIndex);
        bool BroadcastMsg(MSG_BASE & rMsg);

        bool GetMsg(MSG_BASE * &pMsg, unsigned int & nSlotIndex);
        void DisconnectClient(unsigned int nSlotIndex, int nDisconnectReason);
        void DisconnectAllClient(int nDisconnectReason);
        void Stop();

    protected:
        virtual void Process();

    private:
        bool _AddEvent(int nFd, unsigned int nEvents, CSocketSlot * pSocketSlot);
        bool _ModifyEvent(unsigned int nEvents, CSocketSlot * pSocketSlot);
        bool _InitServer();
        bool _AcceptConnection();
        bool _ProcessEpollEvent(struct epoll_event & rEv);
        bool _SendData(CSocketSlot & rSocketSlot);
        bool _RecvData(CSocketSlot & rSocketSlot);
        void _Close(CSocketSlot & rSocketSlot);
        void _Pretreat(MSG_BASE * &pMsg, unsigned int &nSlotIndex);

        bool m_bInitSuccess;
        unsigned short m_nPort;
        int m_nListenerFd;

        int m_nEpollFd;
        unsigned int m_nMaxClient;
        unsigned int m_nCurClient;

        CRecvDataElementArray m_RecvArray;
        CRecvDataElementQueue m_RecvQueue;
        CSocketSlotMgr m_SocketSlotMgr;

        bool m_bGetMsgThreadSafety;
        Mutex m_MutextForGetMsg;

        bool m_bRunning;
    };

}

#endif // __net_socketmgr_h_

