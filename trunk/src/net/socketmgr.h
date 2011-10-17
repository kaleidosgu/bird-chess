#ifndef __net_socketmgr_h_
#define __net_socketmgr_h_

#include "../base/thread.h"
#include "../base/loopqueue.h"
#include "socketslot.h"
#include "recvdataelement.h"
//#include "recvdataelementqueue.h"
//#include "recvdataelementarray.h"
#include <sys/epoll.h>
#include <vector>

using namespace base;
using namespace std;

const unsigned int cMAX_SERVER_PUBLIC_KEY_LEN = 512;
const unsigned int cMAX_SERVER_PRIVATE_KEY_LEN = 512;
const char cSERVER_PUBLIC_KEY[cMAX_SERVER_PUBLIC_KEY_LEN] = "QWERDFADSRFQWER!@#$ASDFASDFG!#@$!%#$T%!#E$RASFDSDG$#@%$@!#$@!#!$!@#$!@#%!@#$@#$!";
const char cSERVER_PRIVATE_KEY[cMAX_SERVER_PRIVATE_KEY_LEN] = "QWERDFADSRFQWER!@#$ASDFASDFG!#@$!%#$T%!#E$RASFDSDG$#@%$@!#$@!#!$!@#$!@#%!@#$@#$!";

namespace net
{
    class CSocketMgr : public Thread
    {
        friend class CSocketSlot;
    public:
        CSocketMgr();
        ~CSocketMgr();
        bool Init(unsigned short nPort, unsigned int nMaxClient, unsigned int nAliveCheckInterval = 5, unsigned int nAliveTimeOut = 30, bool bGetMsgThreadSafety = false);

        bool SendMsg(MSG_BASE & rMsg, unsigned int nSlotIndex);
        bool BroadcastMsg(MSG_BASE & rMsg);

        bool GetMsg(MSG_BASE * &pMsg, unsigned int & nSlotIndex);
        void DisconnectClient(unsigned int nSlotIndex, int nDisconnectReason);
        void DisconnectAllClient(int nDisconnectReason);
        void Stop();

    protected:
        virtual void Process();

    private:
        void _SendData();
        bool _AddEvent(int nFd, unsigned int nEvents, CSocketSlot * pSocketSlot);
        bool _ModifyEvent(unsigned int nEvents, CSocketSlot * pSocketSlot);
        bool _InitServer();
        bool _AcceptConnection();
        bool _ProcessEpollEvent(struct epoll_event & rEv);
        void _Close(CSocketSlot & rSocketSlot);
        void _Pretreat(MSG_BASE * &pMsg, unsigned int &nSlotIndex);

        void _CheckAlive();

        bool m_bInitSuccess;
        unsigned short m_nPort;
        int m_nListenerFd;

        int m_nEpollFd;
        unsigned int m_nMaxClient;

        LoopQueue< CRecvDataElement * > * m_pRecvQueue;
       
        vector< CSocketSlot * > m_SendQueue1;
        vector< CSocketSlot * > m_SendQueue2;
        vector< CSocketSlot * > * m_pAddSendQueue;
        vector< CSocketSlot * > * m_pDelSendQueue;
        Mutex m_MutexForSendQueue;

        CSocketSlotMgr m_SocketSlotMgr;

        bool m_bGetMsgThreadSafety;
        Mutex m_MutextForGetMsg;

        bool m_bRunning;
        time_t m_tLastCheckTime;
        unsigned int m_nAliveCheckInterval; // second
        unsigned int m_nAliveTimeOut; //second
    };

}

#endif // __net_socketmgr_h_

