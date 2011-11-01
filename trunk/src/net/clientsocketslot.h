#ifndef __net_clientsocketslot_h_
#define __net_clientsocketslot_h_

#include "socketslot.h"

namespace net
{
    class CClientSocketSlot : public CSocketSlot
    {
    public:
        CClientSocketSlot();
        virtual ~CClientSocketSlot();
        void OnConnect(int nFd, const sockaddr_in &rSaClient);
        //void SetRecvQueue(CRecvDataElementQueue * pRecvQueue);
        void Init(CRSA * pServerRSA, bool bEncrypt, bool bCompress, LoopQueue< CRecvDataElement * > * pRecvQueue);
        void Reset();
        bool NeedSendData();
    protected:
        virtual bool _DisposeRecvMsg(MSG_BASE & rMsg);
    };
}

#endif // __net_clientsocketslot_h_

