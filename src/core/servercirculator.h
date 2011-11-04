#ifndef __core_servercirculator_h_
#define __core_servercirculator_h_

#include "circulator.h"
#include "../net/socketmgr.h"
#include "../base/ini.h"

using namespace net;
using namespace base;

namespace core
{
    class CServerCirculator : public Circulator
    {
    public:
        CServerCirculator(string strServerName);
        virtual ~CServerCirculator();
        bool SendMsg(MSG_BASE & rMsg, unsigned int nSlotIndex);

    protected:
        virtual bool Initialize();
        virtual void Pump();
        virtual bool PreProcess();
        virtual void EndProcess();
        virtual void ProcessMsg(MSG_BASE & rMsg, unsigned int nSlotIndex) = 0;
        virtual void HeartBeat(unsigned int nIntervalMS);
        virtual void Destory();
    protected:
        bool _InitSocketMgr();

        CSocketMgr m_SocketMgr;
        string m_strServerName;
    };
}

#endif // __core_servercirculator_h_

