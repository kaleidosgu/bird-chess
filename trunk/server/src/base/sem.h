#ifndef _base_sem_h_
#define _base_sem_h_

#include <semaphore.h>

namespace base
{
    class Sem
    {
    public:
        Sem();
        Sem(int nCount);
        ~Sem();
        bool Wait(int nTimeout);
        void Signal();

    private:
        sem_t m_sem;
    };
}

#endif // _base_sem_h_

