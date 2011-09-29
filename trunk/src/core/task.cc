#include "task.h"

namespace core
{
    Task::Task()
    {
        m_TaskState = TaskState_New;
        m_nTaskID = -1;
    }
    
    Task::~Task()
    {
    }
}

