#ifndef __core_taskmanager_h_
#define __core_taskmanager_h_

#include "taskqueue.h"
#include "../base/mutex.h"
#include "../base/condition.h"

using namespace base;

namespace core
{
    //class WorkerManager;
    enum LoadState
    {
        LoadState_Free,
        LoadState_Normal,
        LoadState_Full
    };
    class TaskManager
    {
    public:
        TaskManager();
        //TaskManager(WorkerManager & rWorkerManager);
        virtual ~TaskManager();
        void EnableLockForGetCompletedTask(); // Enable the lock for get completed task
        bool AddTask(Task * pTask, LoadState & rLoadState);
        bool GetTasks(TaskQueue & rTaskQueue, unsigned int nMaxCount);
        bool GetAllTasks(TaskQueue & rTaskQueue);
        void NotifyAllWorkers();

        bool AddCompletedTask(Task * pTask);
        Task * GetCompletedTask();
    private:
        void _CheckLoadState(LoadState & rLoadState);

        TaskQueue m_TaskQueue;
        int m_nFreeWorkerNum;
        Mutex m_MutexForTaskQueue;
        Condition m_CondForWorker;
        //WorkerManager & m_rWorkerManager;
        long long m_nNextTaskID;

        TaskQueue m_CompletedTaskQueueB; //Completed Task Queue Background
        Mutex m_MutexForCTQB; // Mutex For Completed Task Queue Background

        TaskQueue m_CompletedTaskQueue; //Completed Task Queue
        Mutex m_MutexForCTQ; // Mutex For Completed Task Queue
        bool m_bNeedLockForCTQ; // Need Lock For Completed Task Queue
    };

}
#endif // __core_taskmanager_h_

