#include "recvdataelementqueue.h"
#include "recvdataelement.h"
#include "recvdataelementarray.h"
#include "msgbasic.h"

using namespace net;

CRecvDataElementQueue::CRecvDataElementQueue()
{
    m_nMaxQueueSize = 0;
    m_RecvData = NULL;
    m_pHead = NULL;
    m_pTail = NULL;
}

CRecvDataElementQueue::~CRecvDataElementQueue()
{
    delete [] m_RecvData;
    m_RecvData = NULL;
    m_pHead = NULL;
    m_pTail = NULL;
}

bool CRecvDataElementQueue::Init(unsigned int nQueueSize)
{
    m_nMaxQueueSize = nQueueSize;
    m_RecvData = new CRecvDataElement[m_nMaxQueueSize];
    if (m_RecvData == NULL)
    {
        return false;
    }
    m_pHead = m_RecvData;
    m_pTail = m_RecvData;
    return true;
}

CRecvDataElement * CRecvDataElementQueue::GetRecvDataElement()
{
    CRecvDataElement * pRde = NULL;
    if (m_pHead != m_pTail)
    {
        pRde = m_pHead;
        ++m_pHead;
        if (m_pHead == m_RecvData + m_nMaxQueueSize)
        {
            m_pHead = m_RecvData;
        }
    }
    return pRde;
}

bool CRecvDataElementQueue::PutRecvDataElement(const MSG_BASE & rMsg, const unsigned int nSlotIndex)
{
    bool bSuccess = true;
    CRecvDataElement * ptr = m_pTail;
    ++ptr;
    if (ptr == m_RecvData + m_nMaxQueueSize)
    {
        ptr = m_RecvData;
    }
    if (ptr != m_pHead)
    {
        m_pTail->CopySaveData(rMsg, nSlotIndex);
        m_pTail = ptr;
    }
    else
    {
        bSuccess = false;
    }
    return bSuccess;
}

bool CRecvDataElementQueue::MoveRecvDataElementToArray(CRecvDataElementArray & recvDataElementArray)
{
    CRecvDataElement * ptr = m_pTail;
    int nCount = ptr - m_pHead;
    if (nCount > 0)
    {
        if (recvDataElementArray.MoveRecvDataElement(m_pHead, nCount))
        {
            m_pHead = ptr;
        }
        else
        {
            return false;
        }
    }
    else if (nCount < 0)
    {
        if (recvDataElementArray.MoveRecvDataElement(m_pHead, m_RecvData + m_nMaxQueueSize - m_pHead))
        {
            m_pHead = m_RecvData;
            if (recvDataElementArray.MoveRecvDataElement(m_RecvData, ptr - m_RecvData))
            {
                m_pHead = ptr;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    return true;
}

