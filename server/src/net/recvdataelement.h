#ifndef __net_recvdataelement_h_
#define __net_recvdataelement_h_


struct MSG_BASE;
namespace net
{
    class CRecvDataElement
    {
        private:
            MSG_BASE * m_pMsg;
            unsigned int m_nSlotIndex;
        public:
            CRecvDataElement();
            ~CRecvDataElement();
            void SetSlotIndex(unsigned int nSlotIndex);
            void SetData(MSG_BASE * pMsg, unsigned int nSlotIndex);
            void CopySaveData(const MSG_BASE & rMsg, const unsigned int nSlotIndex);
            void DeleteData();
            MSG_BASE * GetMsg() const;
            unsigned int GetSlotIndex() const;
    };
};

#endif // __net_recvdataelement_h_

