#include "recvdataelementarray.h"
#include "recvdataelement.h"
#include "../base/log.h"
#include <string>

using namespace net;
using namespace base;

CRecvDataElementArray::CRecvDataElementArray()
{
    m_RecvData = NULL;
    m_pHead = NULL;
    m_pTail = NULL;
    m_nMaxArraySize = 0;
}

CRecvDataElementArray::~CRecvDataElementArray()
{
    delete [] m_RecvData;
    m_RecvData = NULL;
    m_pHead = NULL;
    m_pTail = NULL;
}

bool CRecvDataElementArray::Init(unsigned int nArraySize)
{
    m_nMaxArraySize = nArraySize;
    m_RecvData = new CRecvDataElement[m_nMaxArraySize];
    m_pHead = m_RecvData;
    m_pTail = m_RecvData;
    return true;
}

CRecvDataElement * CRecvDataElementArray::GetRecvDataElement()
{
    CRecvDataElement * pRde = NULL;
    if (m_pHead < m_pTail)
    {
        pRde = m_pHead;
        ++m_pHead;
        if (m_pHead == m_pTail)
        {
            m_pHead = m_RecvData;
            m_pTail = m_RecvData;
        }
    }
    return pRde;
}

bool CRecvDataElementArray::MoveRecvDataElement(CRecvDataElement * pRecvData, unsigned int nNumber)
{
    if (!_CanAddRecvDataElement() || (m_pTail - m_pHead + nNumber) > (m_nMaxArraySize - 1))
    {
        WriteLog(LEVEL_WARNING, "CRecvDataElementArray::MoveRecvDataElement\n");
        return false;
    }
    else
    {
        memcpy(m_pTail, pRecvData, sizeof(CRecvDataElement) * nNumber);
        m_pTail += nNumber;
        return true;
    }
}

bool CRecvDataElementArray::MoveRecvDataElement(MSG_BASE & rMsg, unsigned int nSlotIndex)
{
    if (!_CanAddRecvDataElement() || (m_pTail - m_pHead + 1) > (m_nMaxArraySize - 1))
    {
        WriteLog(LEVEL_WARNING, "CRecvDataElementArray::MoveRecvDataElement\n");
        return false;
    }
    else
    {
        m_pTail->CopySaveData(rMsg, nSlotIndex);
        m_pTail += 1;
        return true;
    }
}

bool CRecvDataElementArray::IsEmpty()
{
    return (m_pHead == m_pTail);
}

bool CRecvDataElementArray::_CanAddRecvDataElement()
{
    return (m_pHead == m_RecvData);
}

