#ifndef __login_loginmsg_h_
#define __login_loginmsg_h_

#include "../net/msgbasic.h"
#include "../net/systemmsg.h"

enum
{
    MSGID_LOGIN_Begin = 0x1000,
    MSGID_LOGIN_C2S_LoginRequest,
    MSGID_LOGIN_S2C_LoginResult,
    MSGID_LOGIN_LA2LC_LoginRequest,
    MSGID_LOGIN_LC2LA_LoginResult,
};

const unsigned int cMAX_USERNAME_LEN = 51;
const unsigned int cMAX_PASSWORD_LEN = 21;
struct MSG_LOGIN_C2S_LoginRequest : public MSG_BASE
{
    MSG_LOGIN_C2S_LoginRequest()
    {
        nMsg = MSGID_LOGIN_C2S_LoginRequest;
        nSize = sizeof(MSG_LOGIN_C2S_LoginRequest);
        memset(szUsername, 0, cMAX_USERNAME_LEN);
        memset(szPassword, 0, cMAX_PASSWORD_LEN);
    }
    char szUsername[cMAX_USERNAME_LEN];
    char szPassword[cMAX_PASSWORD_LEN];
};

struct MSG_LOGIN_S2C_LoginResult : public MSG_BASE
{
    enum Result
    {
        Result_Success,
        Result_Relogin,
        Result_Failed,
        Result_SendRequest2LoginCenterFailed,
        Result_DisconnectWithLoginCenter,
        Result_LoginRequestDBQueryIsNULL,
        Result_TheLoginRequestDBQueryNumberError,
        Result_CreateUserDBQueryIsNULL,
        Result_TheCreateUserDBQueryNumberError,
    };
    MSG_LOGIN_S2C_LoginResult()
    {
        nMsg = MSGID_LOGIN_S2C_LoginResult;
        nSize = sizeof(MSG_LOGIN_S2C_LoginResult);
        nResult = Result_Success;
    }
    int nResult;
};

struct MSG_LOGIN_LA2LC_LoginRequest : public MSG_BASE
{
    MSG_LOGIN_LA2LC_LoginRequest()
    {
        nMsg = MSGID_LOGIN_LA2LC_LoginRequest;
        nSize = sizeof(MSG_LOGIN_LA2LC_LoginRequest);
        nSlotIndex = 0;
        nSeqID = 0;
    }
    unsigned int nSlotIndex; 
    long long nSeqID;
    MSG_LOGIN_C2S_LoginRequest lrMsg;
};

struct MSG_LOGIN_LC2LA_LoginResult : public MSG_BASE
{
    MSG_LOGIN_LC2LA_LoginResult()
    {
        nMsg = MSGID_LOGIN_LC2LA_LoginResult;
        nSize = sizeof(MSG_LOGIN_LC2LA_LoginResult);
        nSlotIndex = 0;
        nSeqID = 0;
    }
    unsigned int nSlotIndex; 
    long long nSeqID;
    MSG_LOGIN_S2C_LoginResult lrMsg;
};

#endif // __login_loginmsg_h_
