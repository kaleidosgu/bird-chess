#include "taskqueue.h"

namespace core
{
    TaskQueue::TaskQueue()
    {
    }

    TaskQueue::~TaskQueue()
    {
    }

    bool TaskQueue::AddTask(Task * pTask)
    {
        m_TaskQueue.AddBack(pTask);
        return true;
    }

    bool TaskQueue::GetTasks(TaskQueue & rTaskQueue, unsigned int nMaxCount)
    {
        return m_TaskQueue.GetElements(rTaskQueue.m_TaskQueue, nMaxCount);
    }

    bool TaskQueue::GetAllTasks(TaskQueue & rTaskQueue)
    {
        return m_TaskQueue.GetAllElements(rTaskQueue.m_TaskQueue);
    }

    Task * TaskQueue::GetTask()
    {
        return m_TaskQueue.GetFront();
    }

    unsigned int TaskQueue::GetTaskNum()
    {
        return m_TaskQueue.GetCount();
    }

    bool TaskQueue::IsEmpty()
    {
        return (m_TaskQueue.GetCount() == 0);
    }
}
