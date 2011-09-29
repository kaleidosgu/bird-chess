#include "taskmanager.h"
#include "../base/log.h"
#include "../base/thread.h"

namespace core
{
    TaskManager::TaskManager()
        : m_CondForWorker(m_MutexForTaskQueue)
    {
        m_bNeedLockForCTQ = false;
        m_nNextTaskID = 0;
        m_nFreeWorkerNum = 0;
    }

    /*
       TaskManager::TaskManager(WorkerManager & rWorkerManager)
       : m_CondForWorker(m_MutexForTaskQueue)
       , m_rWorkerManager(rWorkerManager)
       {
       }
     */

    TaskManager::~TaskManager()
    {
        while (!m_TaskQueue.IsEmpty())
        {
            WriteLog(LEVEL_DEBUG, "The task queue is not empty, please wait for a moment.\n");
            Thread::Sleep(100);
        }

        WriteLog(LEVEL_DEBUG, "The task queue is empty now.\n");
    }
    
    void TaskManager::EnableLockForGetCompletedTask()
    {
        m_bNeedLockForCTQ = true;
    }

    bool TaskManager::AddTask(Task * pTask, LoadState & rLoadState)
    {
        Mutex::ScopedLock lock(m_MutexForTaskQueue);
        pTask->m_nTaskID = m_nNextTaskID;
        m_nNextTaskID++;
        if (m_nNextTaskID < 0)
        {
            m_nNextTaskID = 0;
        }
        bool bRes = m_TaskQueue.AddTask(pTask);
        if (m_nFreeWorkerNum > 0)
        {
            m_nFreeWorkerNum--;
            //WriteLog(LEVEL_DEBUG, "FreeWorkerNumber--. %d.\n", m_nFreeWorkerNum);
            m_CondForWorker.Notify();
        }
        _CheckLoadState(rLoadState);
        return bRes;
    }
    void TaskManager::_CheckLoadState(LoadState & rLoadState)
    {
        int nTaskNum = m_TaskQueue.GetTaskNum();
        if (nTaskNum < 100)
        {
            rLoadState = LoadState_Free;
            //if (m_nFreeWorkerNum > 0)
            //{
            //m_rWorkerManager.RequestRemoveWorker();
            //}
        }
        else if (nTaskNum > 3000)
        {
            rLoadState = LoadState_Full;
            //WriteLog("Full load. Tast Number is %d.\n", nTaskNum);
            //WriteLog("Full load and request add worker. %d.\n", m_TaskQueue.GetTaskNum());
            //m_rWorkerManager.RequestAddWorker();
        }
        else
        {
            rLoadState = LoadState_Normal;
        }
    }

    bool TaskManager::GetTasks(TaskQueue & rTaskQueue, unsigned int nMaxCount)
    {
        Mutex::ScopedLock lock(m_MutexForTaskQueue);
        if (m_TaskQueue.IsEmpty())
        {
            m_nFreeWorkerNum++;
            //WriteLog(LEVEL_DEBUG, "FreeWorkerNumber++. %d.\n", m_nFreeWorkerNum);
            m_CondForWorker.Wait();
        }
        //WriteLog(LEVEL_DEBUG, "GetTasks. TaskQueueSize=%d.\n", m_TaskQueue.GetTaskNum());
        return m_TaskQueue.GetTasks(rTaskQueue, nMaxCount);
    }

    bool TaskManager::GetAllTasks(TaskQueue & rTaskQueue)
    {
        Mutex::ScopedLock lock(m_MutexForTaskQueue);
        if (m_TaskQueue.IsEmpty())
        {
            m_nFreeWorkerNum++;
            //WriteLog(LEVEL_DEBUG, "FreeWorkerNumber++. %d.\n", m_nFreeWorkerNum);
            m_CondForWorker.Wait();
        }
        //WriteLog(LEVEL_DEBUG, "GetAllTasks. TaskQueueSize=%d.\n", m_TaskQueue.GetTaskNum());
        return m_TaskQueue.GetAllTasks(rTaskQueue);
    }

    void TaskManager::NotifyAllWorkers()
    {
        Mutex::ScopedLock lock(m_MutexForTaskQueue);
        if (m_nFreeWorkerNum > 0)
        {
            m_nFreeWorkerNum = 0;
            //WriteLog(LEVEL_DEBUG, "FreeWorkerNumber=0. %d.\n", m_nFreeWorkerNum);
            m_CondForWorker.NotifyAll();
        }
    }
    
    bool TaskManager::AddCompletedTask(Task * pTask)
    {
        Mutex::ScopedLock lock(m_MutexForCTQB);
        //WriteLog("size=%d\n",sizeof(pTask));
        return m_CompletedTaskQueueB.AddTask(pTask);
    }

    Task * TaskManager::GetCompletedTask()
    {
        Task * pCompletedTask = NULL;
        if (m_bNeedLockForCTQ)
        {
            m_MutexForCTQ.Lock();
        }

        if (m_CompletedTaskQueue.IsEmpty())
        {
            // Get From Workers
            Mutex::ScopedLock lock(m_MutexForCTQB);
            m_CompletedTaskQueueB.GetAllTasks(m_CompletedTaskQueue);
        }
        pCompletedTask = m_CompletedTaskQueue.GetTask();
        if (m_bNeedLockForCTQ)
        {
            m_MutexForCTQ.Unlock();
        }
        return pCompletedTask;
    }
}

