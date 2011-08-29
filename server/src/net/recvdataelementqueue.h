#ifndef __net_recvdataelementqueue_h_
#define __net_recvdataelementqueue_h_

struct MSG_BASE;

namespace net
{
    class CRecvDataElement;
    class CRecvDataElementArray;
    class CRecvDataElementQueue
    {
        public:
            CRecvDataElementQueue();
            ~CRecvDataElementQueue();
            bool Init(unsigned int nQueueSize);
            CRecvDataElement * GetRecvDataElement();
            bool PutRecvDataElement(const MSG_BASE & rMsg, unsigned int nSlotIndex);
            bool MoveRecvDataElementToArray(CRecvDataElementArray & recvDataElementArray);

        private:
            CRecvDataElement * m_RecvData;
            CRecvDataElement * m_pHead;
            CRecvDataElement * m_pTail;
            unsigned int m_nMaxQueueSize;
    };
}

#endif // __net_recvdataelementqueue_h_

