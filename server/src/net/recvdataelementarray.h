#ifndef __net_recvdataelementarray_h_
#define __net_recvdataelementarray_h_

struct MSG_BASE;

namespace net
{
    class CRecvDataElement;
    class CRecvDataElementArray
    {
        public:
            CRecvDataElementArray();
            ~CRecvDataElementArray();
            bool Init(unsigned int nArraySize);
            CRecvDataElement * GetRecvDataElement();
            bool MoveRecvDataElement(CRecvDataElement * pRecvData, unsigned int nNumber);
            bool MoveRecvDataElement(MSG_BASE & rMsg, unsigned int nSlotIndex);
            bool IsEmpty();
        private:
            bool _CanAddRecvDataElement();

            CRecvDataElement * m_RecvData;
            CRecvDataElement * m_pHead;
            CRecvDataElement * m_pTail;
            unsigned int m_nMaxArraySize;
    };
}

#endif // __net_recvdataelementarray_h_

