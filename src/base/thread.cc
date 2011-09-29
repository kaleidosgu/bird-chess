// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)


#include "thread.h"
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <linux/unistd.h>

namespace base
{
    namespace CurrentThread
    {
        __thread const char* t_threadName = "unknown";
    }

    namespace detail
    {
        __thread pid_t t_cachedTid = 0;

        pid_t gettid()
        {
            return static_cast<pid_t>(::syscall(SYS_gettid));
        }

        void afterFork()
        {
            t_cachedTid = gettid();
            base::CurrentThread::t_threadName = "main";
            // no need to call pthread_atfork(NULL, NULL, &afterFork);
        }

        class ThreadNameInitializer
        {
            public:
                ThreadNameInitializer()
                {
                    base::CurrentThread::t_threadName = "main";
                    pthread_atfork(NULL, NULL, &afterFork);
                }
        };

        ThreadNameInitializer init;
    }
}

using namespace base;
using namespace base::detail;

pid_t CurrentThread::tid()
{
    if (t_cachedTid == 0)
    {
        t_cachedTid = gettid();
    }
    return t_cachedTid;
}

const char* CurrentThread::Name()
{
    return t_threadName;
}

bool CurrentThread::IsMainThread()
{
    return tid() == ::getpid();
}

Thread::Thread(const string & strThreadName)
: m_bStarted(false),
    m_thread(0),
    m_tid(0),
    m_strThreadName(strThreadName)
{
}

Thread::~Thread()
{
    Wait();
}

void * Thread::_ThreadProc( void * lpParam)
{
    Thread * pThread = static_cast<Thread *>(lpParam);
    pThread->_RunThread();
    return NULL;
}

void Thread::Run()
{
    if (!m_bStarted)
    {
        m_bStarted = true;
        pthread_create(&m_thread, NULL, Thread::_ThreadProc, this);
    }
}

void Thread::Wait()
{
    if (m_bStarted && m_thread != 0)
    {
        pthread_join(m_thread, NULL);
        m_thread = 0;
    }
}

void Thread::_RunThread()
{
    m_tid = CurrentThread::tid();
    base::CurrentThread::t_threadName = m_strThreadName.c_str();
    Process();
    base::CurrentThread::t_threadName = "finished";
}

void Thread::Sleep(int nMillisecond)
{
    usleep(1000 * nMillisecond);
}

