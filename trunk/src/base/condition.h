#ifndef __base_condition_h_
#define __base_condition_h_

#include "mutex.h"
#include <pthread.h>


namespace base
{

    class Condition
    {
        public:
            explicit Condition(Mutex& rMutex) : m_Mutex(rMutex)
                                                    
        {
            pthread_cond_init(&m_pcond, NULL);
        }

            ~Condition()
            {
                pthread_cond_destroy(&m_pcond);
            }

            void TimedWait(const struct timespec *abstime)
            {
                pthread_cond_timedwait(&m_pcond, m_Mutex.GetPthreadMutex(), abstime);
            }

            void Wait()
            {
                pthread_cond_wait(&m_pcond, m_Mutex.GetPthreadMutex());
            }

            void Notify()
            {
                pthread_cond_signal(&m_pcond);
            }

            void NotifyAll()
            {
                pthread_cond_broadcast(&m_pcond);
            }

        private:
            Mutex& m_Mutex;
            pthread_cond_t m_pcond;
    };

}
#endif  // __base_condition_h_

