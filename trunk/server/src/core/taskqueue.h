#include "task.h"
#include "../base/dequeue.h"

using namespace base;
namespace core
{
    class TaskQueue
    {
    public:
        TaskQueue();
        virtual ~TaskQueue();
	bool AddTask(Task * pTask);
	bool GetTasks(TaskQueue & rTaskQueue, unsigned int nMaxCount);
	bool GetAllTasks(TaskQueue & rTaskQueue);
	Task * GetTask();
	unsigned int GetTaskNum();
        bool IsEmpty();
    private:
        Dequeue< Task > m_TaskQueue;
    };
}

