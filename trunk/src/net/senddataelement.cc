#include "senddataelement.h"
#include "msgbasic.h"

using namespace net;

CSendDataElement::CSendDataElement()
{
    iov_base = NULL;
    iov_len = 0;
}
CSendDataElement::~CSendDataElement()
{
}

void CSendDataElement::SetData(MSG_BASE * pMsg)
{
    iov_base = (char *)pMsg;
    iov_len = pMsg->nSize;
}

void CSendDataElement::DeleteData()
{
    delete (MSG_BASE *) iov_base;
    iov_base = NULL;
    iov_len = 0;
}

MSG_BASE * CSendDataElement::GetMsgData() const
{
    return (MSG_BASE *)iov_base;
}

unsigned short CSendDataElement::GetMsgDataSize() const
{
    return iov_len;
}

iovec * CSendDataElement::GetIovec()
{
    return this;
}

