#ifndef __net_systemmsg_h_
#define __net_systemmsg_h_

#include "msgbasic.h"

const unsigned short MSGID_TYPE_SYSTEM                    = 0x0000;
const unsigned short MSGID_SYSTEM_ConnectSuccess          = 0x0001;
const unsigned short MSGID_SYSTEM_Disconnect              = 0x0002;
const unsigned short MSGID_SYSTEM_CheckAlive              = 0x0003;
const unsigned short MSGID_SYSTEM_CheckAliveReply         = 0x0004;

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

struct MSG_SYSTEM_CheckAlive : public MSG_BASE
{
    MSG_SYSTEM_CheckAlive()
    {
        nMsg = MSGID_SYSTEM_CheckAlive;
        nSize = sizeof(MSG_SYSTEM_CheckAlive);
    }
};

struct MSG_SYSTEM_CheckAliveReply : public MSG_BASE
{
    MSG_SYSTEM_CheckAliveReply()
    {
        nMsg = MSGID_SYSTEM_CheckAliveReply;
        nSize = sizeof(MSG_SYSTEM_CheckAliveReply);
    }
};

#pragma pack()

#endif // __net_systemmsg_h_

