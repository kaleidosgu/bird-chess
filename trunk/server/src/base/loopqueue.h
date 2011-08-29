#ifndef __base_loopqueue_h_
#define __base_loopqueue_h_

namespace base
{
    template<class T>
        class LoopQueue
        {
        public:
            LoopQueue(unsigned int nMaxSize) : m_nMaxSize(nMaxSize)
            {
                m_aData = new T()[nMaxSize];
                m_pHead = m_aData;
                m_pTail = m_aData;
            }
            virtual ~LoopQueue()
            {
                m_pHead = NULL;
                m_pTail = NULL;
                delete [] m_aData;
                m_aData = NULL;
            }
            T * GetElement()
            {
                T * pElement = NULL;
                if (m_pHead != m_pTail)
                {
                    pElement = m_pHead;
                    ++m_pHead;
                    if (m_pHead == m_RecvData + m_nMaxSize)
                    {
                        m_pHead = m_RecvData;
                    }
                }
                return pElement;
            }
            bool AddElement(T & rElement)
            {
                bool bSuccess = true;
                T * ptr = m_pTail;
                ++ptr;
                if (ptr == m_RecvData + m_nMaxSize)
                {
                    ptr = m_RecvData;
                }
                if (ptr != m_pHead)
                {
                    *m_pTail = rElement;
                    m_pTail = ptr;
                }
                else
                {
                    bSuccess = false;
                }
                return bSuccess;
            }
        private:
            T * m_aData;
            T * m_pHead;
            T * m_pTail;
            const unsigned int m_nMaxSize;
        };
}

#endif // __base_loopqueue_h_

