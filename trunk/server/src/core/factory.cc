#include "factory.h"
#include "../base/log.h"
namespace core
{
    Factory::Factory()
        : m_WorkerManager(m_TaskManager)
    {
        m_nMaxWorkerNum = 1;
    }
    Factory::Factory(unsigned int nMaxWorkerNum)
        : m_WorkerManager(m_TaskManager)
    {
        m_nMaxWorkerNum = nMaxWorkerNum == 0 ? 1 : nMaxWorkerNum;
    }

    Factory::Factory(unsigned int nMaxWorkerNum, unsigned int nMaxTaskNumPerWorker)
        : m_WorkerManager(m_TaskManager, nMaxTaskNumPerWorker)
    {
        m_nMaxWorkerNum = nMaxWorkerNum == 0 ? 1 : nMaxWorkerNum;
    }

    Factory::~Factory()
    {
    }

    bool Factory::Init()
    {
        bool bRes = true;
        for (unsigned int i=0; i<m_nMaxWorkerNum; i++)
        {
            Worker * pNewWorker = RecruitWorker();
            m_WorkerManager.AddWorker(pNewWorker);
            if (!pNewWorker->Init())
            {
                bRes = false;
            }
            else
            {
                pNewWorker->Run();
            }
        }
        return bRes;
    }

    /*
    Worker * Factory::RecruitWorker()
    {
        return new Worker();
    }
    */

    bool Factory::AddTask(Task * pTask)
    {
        if (pTask == NULL)
        {
            WriteLog(LEVEL_WARNING, "Why add null task.\n");
            return false;
        }
        LoadState loadState = LoadState_Free;
        bool bRes = m_TaskManager.AddTask(pTask, loadState);

        /*
           if (m_nCurWorkerNum == 0)
           {
           _AddWorker();
           }
           if (loadState == LoadState_Free && m_nCurWorkerNum > 1)
           {
           _RemoveWorker();
           }
           else if (loadState == LoadState_Full || )
           {
           _AddWorker();
           }
         */
        return bRes;
    }

    Task * Factory::GetCompletedTask()
    {
        return m_TaskManager.GetCompletedTask();
    }
    /*
       void Factory::_AddWorker()
       {
       }

       bool Factory::_RemoveWorker()
       {
       return true;
       }
     */
}
