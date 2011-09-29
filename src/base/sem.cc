#include "sem.h"
#include <sys/time.h>

namespace base
{
    Sem::Sem()
    {
        sem_init(&m_sem, 0, 0);
    }

    Sem::Sem(int nCount)
    {
        sem_init(&m_sem, 0, nCount);
    }

    Sem::~Sem()
    {
        sem_destroy(&m_sem);
    }

    bool Sem::Wait(int nTimeout)
    {
        if (nTimeout < 0)
        {
            if (sem_wait(&m_sem) == 0)
            {
                return true;
            }
        }
        else
        {
            struct timeval tv;
            struct timespec abstime;
            gettimeofday(&tv, 0);
            const long oneBillion = 1000000000;
            abstime.tv_sec = tv.tv_sec + (nTimeout / 1000);
            abstime.tv_nsec = (tv.tv_usec * 1000) + ((nTimeout % 1000) * 1000000);
            if (abstime.tv_nsec >= oneBillion)
            {
                ++abstime.tv_sec;
                abstime.tv_nsec -= oneBillion;
            }

            if (sem_timedwait(&m_sem, &abstime) == 0)
            {
                return true;
            }
        }
        return false;
    }


    void Sem::Signal()
    {
        sem_post(&m_sem);
    }
}

