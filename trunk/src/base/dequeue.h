#ifndef __base_dequeue_h_
#define __base_dequeue_h_

#include <deque>

namespace base
{
    template<class T>
        class Dequeue
        {
        public:
            Dequeue(){};
            virtual ~Dequeue(){};
            void ResetQueue()
            {
                if (m_deque.size() == 0)
                {
                    return;
                }
                typename std::deque< T * >::iterator it = m_deque.begin();
                typename std::deque< T * >::iterator itEnd = m_deque.end();
                for (; it != itEnd; ++it)
                {
                    delete (*it);
                }
                m_deque.clear();
            }

            T * GetFront()
            {
                if (m_deque.empty())
                {
                    return NULL;
                }
                T * pElement = m_deque.front();
                m_deque.pop_front();
                return pElement;
            }

            bool GetAllElements(Dequeue< T > & rDesDequeue)
            {
                if (m_deque.size() == 0)
                {
                    return false;
                }
                rDesDequeue.m_deque.insert(rDesDequeue.m_deque.end(), m_deque.begin(), m_deque.end());
                m_deque.clear();
                return true;
            }

            bool GetElements(Dequeue< T > & rDesDequeue, unsigned int nMaxCount)
            {
                if (nMaxCount == 0)
                {
                    return true;
                }
                if (m_deque.size() == 0)
                {
                    return false;
                }
                if (nMaxCount < m_deque.size())
                {
                    rDesDequeue.m_deque.insert(rDesDequeue.m_deque.end(), m_deque.begin(), m_deque.begin() + nMaxCount);
                    m_deque.erase(m_deque.begin(), m_deque.begin() + nMaxCount);
                }
                else
                {
                    rDesDequeue.m_deque.insert(rDesDequeue.m_deque.end(), m_deque.begin(), m_deque.end());
                    m_deque.clear();
                }
                return true;
            }

            void AddBack(T *pElement)
            {
                m_deque.push_back(pElement);
            }

            void AddFront(T *pElement)
            {
                m_deque.push_front(pElement);
            }

            void ClearQueue()
            {
                m_deque.clear();
            }

            unsigned int GetCount()
            {
                return m_deque.size();
            }
        private:
            std::deque< T * > m_deque;
        };
}

#endif // __base_dequeue_h_

