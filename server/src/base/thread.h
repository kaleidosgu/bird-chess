#ifndef __base_thread_h_
#define __base_thread_h_

#include <pthread.h>
#include <string>
using namespace std;

namespace base
{

    class Thread
    {
        public:
            Thread(const string& strThreadName = string());
            virtual ~Thread();
            void Run();
            void Wait();
            virtual void Process() = 0;
            static void Sleep(int nMillisecond);

            bool IsStarted() const { return m_bStarted; }
            pid_t tid() const { return m_tid; }
            const string& Name() const { return m_strThreadName; }

        private:
            static void * _ThreadProc( void * pParam);
            void _RunThread();

            bool       m_bStarted;
            pthread_t  m_thread;
            pid_t      m_tid;
            string     m_strThreadName;
    };

    namespace CurrentThread
    {
        pid_t tid();
        const char* Name();
        bool IsMainThread();
    }

}
#endif // __base_thread_h_

