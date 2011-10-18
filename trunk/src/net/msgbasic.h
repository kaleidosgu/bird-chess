#ifndef __net_msgbasic_h_
#define __net_msgbasic_h_

#include <new>
#include <assert.h>

const unsigned short cMAX_MSG_SIZE = 65530;
const unsigned short cMAX_LOGIC_MSG_SIZE = 64512;

#pragma pack(1)

typedef struct MSG_BASE
{
    unsigned short nMsg;
    unsigned short nSize;
    MSG_BASE()
    {
        nMsg = 0;
        nSize = sizeof(MSG_BASE);
    }
    /*
    unsigned int CheckSum()
    {
        unsigned int nCheckSum = 0;
        unsigned char *ptr = (unsigned char *)this;
        for (unsigned short i=0; i<nSize; ++i)
        {
            nCheckSum += *ptr++;
        }
        return nCheckSum;
    }
    */
} *LPMSG_BASE;

template < typename T >
T * CreateDynamicLengthMsg(unsigned short nSize, const T*)
{
    T *pMsg = (T *)new char[nSize];
    if (pMsg != NULL)
    {
        new ((void *)pMsg)T();
        pMsg->nSize = nSize;
    }
    return pMsg;
};

template < typename T >
T * CreateDynamicLengthMsg2(void * pBuff, unsigned short nSize, const T*)
{
    T *pMsg = NULL;
    if (pBuff != NULL)
    {
        T *pMsg = new(pBuff) T;
        pMsg->nSize = nSize;
    }
    return pMsg;
};

#pragma pack()

#endif // __net_msgbasic_h_

