#include "recvdataelement.h"
#include "msgbasic.h"
#include <string>

using namespace net;

CRecvDataElement::CRecvDataElement()
{
    m_pMsg = NULL;
    m_nSlotIndex = -1;
}

CRecvDataElement::~CRecvDataElement()
{
}

void CRecvDataElement::SetSlotIndex(unsigned int nSlotIndex)
{
    m_nSlotIndex = nSlotIndex;
}

void CRecvDataElement::SetData(MSG_BASE * pMsg, unsigned int nSlotIndex)
{
    m_pMsg = pMsg;
    m_nSlotIndex = nSlotIndex;
}

void CRecvDataElement::CopySaveData(const MSG_BASE & rMsg, const unsigned int nSlotIndex)
{
    m_pMsg = CreateDynamicLengthMsg(rMsg.nSize, (MSG_BASE *)0);
    memcpy(m_pMsg, &rMsg, rMsg.nSize);
    m_nSlotIndex = nSlotIndex;
}

void CRecvDataElement::DeleteData()
{
    delete m_pMsg;
    m_pMsg = NULL;
    m_nSlotIndex = -1;
}

MSG_BASE * CRecvDataElement::GetMsg() const
{
    return m_pMsg;
}

unsigned int CRecvDataElement::GetSlotIndex() const
{
    return m_nSlotIndex;
}

