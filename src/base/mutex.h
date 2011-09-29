#ifndef __base_mutex_h_ 
#define __base_mutex_h_

#include <pthread.h>

namespace base
{

    class Mutex
    {
    public:
        class ScopedLock
        {
        public:
            explicit ScopedLock(Mutex& mutex) : m_mutex(mutex)
            {
                m_mutex.Lock();
            }

            ~ScopedLock()
            {
                m_mutex.Unlock();
            }

        private:

            Mutex& m_mutex;
        };
        Mutex()
        {
            pthread_mutex_init(&m_mutex, NULL);
        }

        ~Mutex()
        {
            pthread_mutex_destroy(&m_mutex);
        }

        void Lock()
        {
            pthread_mutex_lock(&m_mutex);
        }

        void Unlock()
        {
            pthread_mutex_unlock(&m_mutex);
        }

        pthread_mutex_t* GetPthreadMutex() /* non-const */
        {
            return &m_mutex;
        }

    private:

        pthread_mutex_t m_mutex;
    };

}

#define ScopedLock(x) error "Missing guard object name"

#endif  // __base_mutex_h_

