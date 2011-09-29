#ifndef __core_worker_h_
#define __core_worker_h_

#include "../base/thread.h"
#include "taskmanager.h"

using namespace base;

namespace core
{
    class WorkerManager;
    class Worker : public Thread
    {
        friend class WorkerManager;
    public:
        Worker();
        virtual ~Worker();
        virtual bool Init();
        virtual void Process();
        void StopWorking();
    protected:
        virtual void DoTask(Task & rTask) = 0;
        void _SetTaskManager(TaskManager * pTaskManager);
        void _SetMaxTaskNum(unsigned int nMaxTaskNum);

        bool m_bWorking;
        bool m_bIll;
        int m_nWorkerID;
        TaskQueue m_TaskQueue;
        TaskManager * m_pTaskManager;
        unsigned int m_nMaxTaskNum;
    };
}
#endif // __core_worker_h_

