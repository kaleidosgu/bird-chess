#ifndef __core_circulator_h_
#define __core_circulator_h_

#include "../base/thread.h"
#include "../base/condition.h"
#include <sys/timeb.h>

using namespace base;

namespace core
{
    class Circulator : public Thread
    {
    public:
        Circulator();
        virtual ~Circulator();
        bool Start(bool bCreateThread = false);
    protected:
        void SetPeriod(unsigned int nPeriodMS);
        virtual bool Initialize();
        virtual bool PreProcess();
        virtual void Pump();
        virtual void HeartBeat(unsigned int nIntervalMS) = 0;
        virtual void Process();
        virtual void EndProcess();
        virtual void Destory(){}
        void Stop();
    private:
        //CCondition m_Condition;
        //MutexLock m_Mutex;
        unsigned int m_nPeriodMS; //ms
        bool m_bRunning;
        timeb m_tbLast;
    };
}

#endif // __core_circulator_h_
