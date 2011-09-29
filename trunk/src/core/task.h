#ifndef __core_task_h_
#define __core_task_h_

namespace core
{
    class TaskManager;
    class Task
    {
        friend class TaskManager;
    public:
        enum TaskState
        {
            TaskState_New,
            TaskState_Success,
            TaskState_Failed,
            TaskState_Error,
        };

        Task();
        virtual ~Task();

        void SetTaskState(TaskState eTaskState)
        {
            m_TaskState = eTaskState;
        }

        TaskState GetTaskState()
        {
            return m_TaskState;
        }

        long long GetTaskID()
        {
            return m_nTaskID;
        }

    private:
        long long m_nTaskID;
        TaskState m_TaskState;
    };
}
#endif // __core_task_h_

