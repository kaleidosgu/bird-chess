#ifndef __core_workermanager_h_
#define __core_workermanager_h_

#include "taskmanager.h"
#include "worker.h"
#include <vector>

using namespace std;

namespace core
{
    class WorkerManager
    {
    public:
        WorkerManager(TaskManager & rTaskManager);
        WorkerManager(TaskManager & rTaskManager, unsigned int nMaxTaskNumPerWorker);
        virtual ~WorkerManager();
        bool AddWorker(Worker * pWorker);
    private:
        void _StopWorking();

        vector< Worker * > m_WorkerVector;
        unsigned int m_nMaxTaskNumPerWorker;
        TaskManager & m_rTaskManager;
        Mutex m_MutexForWorkerVector;
    };
}
#endif // __core_workermanager_h_

