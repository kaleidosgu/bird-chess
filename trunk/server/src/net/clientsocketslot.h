#ifndef __net_clientsocketslot_h_
#define __net_clientsocketslot_h_

#include "socketslot.h"

namespace net
{
    class CClientSocketSlot : public CSocketSlot
    {
    public:
        CClientSocketSlot();
        ~CClientSocketSlot();
        void OnConnect(int nFd, const sockaddr_in &rSaClient);
        void SetRecvQueue(CRecvDataElementQueue * pRecvQueue);
        void Reset();
    };
}

#endif // __net_clientsocketslot_h_

