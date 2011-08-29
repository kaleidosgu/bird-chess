#ifndef __base_threadsafedeque_h_
#define __base_threadsafedeque_h_

namespace base
{
    template<class T>
        class ThreadSafeDequeue
        {
        public:
            ThreadSafeDequeue() : m_nCount(0){};
            virtual ~CThreadSafeDeque(){};
            void ResetQueue()
            {
                if (m_deque.size() == 0)
                {
                    return;
                }
                Mutex::ScopedLock lock(m_Mutex);
                typename deque< T * >::iterator it = m_deque.begin();
                typename deque< T * >::iterator itEnd = m_deque.end();
                for (; it != itEnd; ++it)
                {
                    delete (*it);
                }
                m_deque.clear();
                m_nCount = 0;
            }

            T *GetFront()
            {
                Mutex::ScopedLock lock(m_Mutex);
                if (m_deque.empty())
                {
                    return NULL;
                }
                T * pElement = m_deque.front();
                m_deque.pop_front();
                m_nCount--;
                return pElement;
            }

            void GetAll(std::deque< T * > & rDesQueue)
            {
                Mutex::ScopedLock lock(m_Mutex);
                rDesQueue.insert(rDesQueue.end(), m_deque.begin(), m_deque.end());
                m_deque.clear();
                m_nCount = 0;
            }

            void AddBack(T *pElement)
            {
                Mutex::ScopedLock lock(m_Mutex);
                m_deque.push_back(pElement);
                m_nCount++;
            }

            void AddFront(T *pElement)
            {
                Mutex::ScopedLock lock(m_Mutex);
                m_deque.push_front(pElement);
                m_nCount++;
            }

            void ClearQueue()
            {
                Mutex::ScopedLock lock(m_Mutex);
                m_deque.clear();
                m_nCount = 0;
            }

            unsigned int GetCurCount()
            {
                Mutex::ScopedLock lock(m_Mutex);
                return m_nCount;
            }
        private:
            std::deque< T * > m_deque;
            unsigned int m_nCount;
            Mutex m_Mutex;
        };
}

#endif // __base_threadsafedeque_h_

