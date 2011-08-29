#include "worker.h"
#include "../base/log.h"

namespace core
{
    Worker::Worker()
    {
        m_pTaskManager = NULL;
        m_bIll = false;
        m_bWorking = false;
        m_nWorkerID = 0;
        m_nMaxTaskNum = 0;
    }

    Worker::~Worker()
    {
    }

    void Worker::StopWorking()
    {
        m_bWorking = false;
    }
    bool Worker::Init()
    {
        return true;
    }

    void Worker::Process()
    {
        if (m_pTaskManager == NULL)
        {
            WriteLog(LEVEL_ERROR, "You have not set the taskmanager for this worker, so ther worker can't run.\n");
            return;
        }
        WriteLog("Hello. I am a worker. I am working now. WorkerID=%d.\n", m_nWorkerID);
        m_bWorking = true;
        Task * pTask = NULL;
        bool bRes = true;
        while (true)
        {
            pTask = m_TaskQueue.GetTask();
            if (pTask)
            {
                DoTask(*pTask);
                m_pTaskManager->AddCompletedTask(pTask);
                //delete pTask;
                //pTask = NULL;
            }
            else
            {
                if (!m_bWorking)
                {
                    break;
                }
                if (m_nMaxTaskNum == 0)
                {
                    bRes = m_pTaskManager->GetAllTasks(m_TaskQueue);
                    //WriteLog(LEVEL_DEBUG, "GetAllTasks. MaxTaskNum=%d, TaskQueueSize=%d, WorkerID=%d.\n", m_nMaxTaskNum, m_TaskQueue.GetTaskNum(), m_nWorkerID);
                }
                else
                {
                    bRes = m_pTaskManager->GetTasks(m_TaskQueue, m_nMaxTaskNum);
                    //WriteLog(LEVEL_DEBUG, "GeTasks. MaxTaskNum=%d, TaskQueueSize=%d, WorkerID=%d.\n", m_nMaxTaskNum, m_TaskQueue.GetTaskNum(), m_nWorkerID);
                }
            }
            if (m_bIll)
            {
                break;
                WriteLog(LEVEL_WARNING, "The worker is ill. WorkerID=%d.\n", m_nWorkerID);
            }
        }

        WriteLog("I am a worker, I am stopping now. WorkerID=%d.\n", m_nWorkerID);
    }
    /*
    void Worker::DoTask(Task & rTask)
    {
        //WriteLog("I am working now. Name=%s. The task ID is %d.\n", m_strName.c_str(), pTask->m_nTaskID);
    }
    */
    
    void Worker::_SetTaskManager(TaskManager * pTaskManager)
    {
        m_pTaskManager = pTaskManager;
    }
    
    void Worker::_SetMaxTaskNum(unsigned int nMaxTaskNum)
    {
        m_nMaxTaskNum = nMaxTaskNum;
    }
}

