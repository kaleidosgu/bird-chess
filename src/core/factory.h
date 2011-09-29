#ifndef __core_factory_h_
#define __core_factory_h_

#include "taskmanager.h"
#include "workermanager.h"

namespace core
{
    class Factory
    {
    public:
        Factory();
        Factory(unsigned int nMaxWorkerNum);
        Factory(unsigned int nMaxWorkerNum, unsigned int nMaxTaskNumPerWorker);
        virtual ~Factory();
        bool Init();
        bool AddTask(Task * pTask);
        
        Task * GetCompletedTask();
    protected:
        virtual Worker * RecruitWorker() = 0;
    private:
        unsigned int m_nMaxWorkerNum;
        WorkerManager m_WorkerManager;
        TaskManager m_TaskManager;
    };
}
#endif // __core_factory_h_

