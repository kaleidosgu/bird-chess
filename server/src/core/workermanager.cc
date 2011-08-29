#include "workermanager.h"
#include "../base/log.h"

namespace core
{
    // class WorkerManager

    WorkerManager::WorkerManager(TaskManager & rTaskManager)
        : m_rTaskManager(rTaskManager)
    {
        m_nMaxTaskNumPerWorker = 0;
    }

    WorkerManager::WorkerManager(TaskManager & rTaskManager, unsigned int nMaxTaskNumPerWorker)
        : m_rTaskManager(rTaskManager)
    {
        m_nMaxTaskNumPerWorker = nMaxTaskNumPerWorker;
    }

    WorkerManager::~WorkerManager()
    {
        Mutex::ScopedLock lock(m_MutexForWorkerVector);
        unsigned int nVectorSize = m_WorkerVector.size();
        for (unsigned int i=0; i<nVectorSize; i++)
        {
            m_WorkerVector[i]->StopWorking();
        }
        m_rTaskManager.NotifyAllWorkers();
        for (unsigned int i=0; i<nVectorSize; i++)
        {
            m_WorkerVector[i]->Wait();
        }
        for (unsigned int i=0; i<nVectorSize; i++)
        {
            delete m_WorkerVector[i];
            m_WorkerVector[i] = NULL;
        }
        m_WorkerVector.clear();
    }

    bool WorkerManager::AddWorker(Worker * pWorker)
    {
        if (pWorker == NULL)
        {
            return false;
        }
        else
        {
            pWorker->_SetTaskManager(&m_rTaskManager);
            m_MutexForWorkerVector.Lock();
            pWorker->m_nWorkerID = m_WorkerVector.size();
            pWorker->_SetMaxTaskNum(m_nMaxTaskNumPerWorker);
            m_WorkerVector.push_back(pWorker);
            m_MutexForWorkerVector.Unlock();
            return true;
        }
    }
}

