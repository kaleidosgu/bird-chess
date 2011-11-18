#include "../../net/clientsocketmgr.h"
#include "../../base/log.h"
#include "../../net/systemmsg.h"
#include "../../login/loginmsg.h"
#include <stdio.h>

using namespace net;

int main()
{
    CClientSocketMgr csm;
    csm.Init(true, true);
    csm.Connect("127.0.0.1", 8887);
    csm.Run();

    MSG_BASE * pMsg = NULL;
    int i=0;
    char szInput[1024];
    for (;;)
    {
        if (i > 1)
        {
            Thread::Sleep(500);
            i = 0;
        }
        i++;
        if (csm.IsDisconnected())
        {
            csm.Reconnect();
            WriteLog("Reconnect.\n");
            Thread::Sleep(1000);
        }
        pMsg = csm.GetMsg();
        if (pMsg != NULL)
        {
            WriteLog("Get the msg. msgid=%d.\n", pMsg->nMsg);
            delete pMsg;
            pMsg = NULL;
        }
        int nLen = 0;
        while (nLen < 1024)
        {
            szInput[nLen] = fgetc(stdin);
            if(szInput[nLen] == EOF)
            {
                base::Thread::Sleep(10);
            }
            if (szInput[nLen] == '\n')
            {
                szInput[nLen] = 0;
                if (nLen > 0)
                {
                    break;
                }
                else
                {
                    continue;
                }
            }
            nLen++;
        }

        string strInput = szInput;
        if (strInput == "a")
        {
            MSG_LOGIN_C2S_LoginRequest lrMsg;
            memcpy(lrMsg.szUsername, "123", 3);
            memcpy(lrMsg.szPassword, "123", 3);
            csm.SendMsg(lrMsg);
        }
        else if (strInput =="q")
        {
            break;
        }
    }
    MSG_BASE msg;
    msg.nMsg = 1121;
    csm.SendMsg(msg);
    csm.Disconnect();
    csm.Wait();
    return 0;
}

