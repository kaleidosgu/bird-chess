#ifndef __net_systemmsg_h_
#define __net_systemmsg_h_

#include "msgbasic.h"

const unsigned short MSGID_TYPE_SYSTEM                    = 0x0000;
const unsigned short MSGID_SYSTEM_ConnectSuccess          = 0x0001;
const unsigned short MSGID_SYSTEM_Disconnect              = 0x0002;

#pragma pack(1)

struct MSG_SYSTEM_ConnectSuccess : public MSG_BASE
{
    MSG_SYSTEM_ConnectSuccess()
    {
        nMsg = MSGID_SYSTEM_ConnectSuccess;
        nSize = sizeof(MSG_SYSTEM_ConnectSuccess);
    }
};

struct MSG_SYSTEM_Disconnect : public MSG_BASE
{
    MSG_SYSTEM_Disconnect()
    {
        nMsg = MSGID_SYSTEM_Disconnect;
        nSize = sizeof(MSG_SYSTEM_Disconnect);
    }
};

#pragma pack()

#endif // __net_systemmsg_h_

