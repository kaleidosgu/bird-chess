#ifndef __base_loopqueue_h_
#define __base_loopqueue_h_

#include <stddef.h>

namespace base
{
    template<class T>
        class LoopQueue
        {
        public:
            LoopQueue(unsigned int nMaxSize) : m_nMaxSize(nMaxSize)
            {
                m_aData = new T[m_nMaxSize];
                m_pBegin = &m_aData[0];
                m_pEnd = &m_aData[m_nMaxSize-1];
                m_pHead = m_pBegin;
                m_pTail = m_pBegin;
            }
            virtual ~LoopQueue()
            {
                m_pHead = NULL;
                m_pTail = NULL;
                m_pBegin = NULL;
                m_pEnd = NULL;
                delete [] m_aData;
                m_aData = NULL;
            }
            bool GetElement(T & rElement)
            {
                if (m_pHead != m_pTail)
                {
                    rElement = *m_pHead;
                    if (m_pHead == m_pEnd)
                    {
                        m_pHead = m_pBegin;
                    }
                    else
                    {
                        m_pHead++;
                    }
                    return true;
                }
                else
                {
                    return false;
                }
            }
            bool AddElement(const T & rElement)
            {
                T * pTailNext = m_pTail;
                if (pTailNext == m_pEnd)
                {
                    pTailNext = m_pBegin;
                }
                else
                {
                    pTailNext++;
                }

                if (pTailNext != m_pHead)
                {
                    *m_pTail = rElement;
                    m_pTail = pTailNext;
                    return true;
                }
                else
                {
                    return false;
                }
            }
        private:
            T * m_aData;
            T * m_pBegin;
            T * m_pEnd;
            T * m_pHead;
            T * m_pTail;
            const unsigned int m_nMaxSize;
        };
}

#endif // __base_loopqueue_h_

