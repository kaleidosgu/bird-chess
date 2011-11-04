#include "../../net/clientsocketmgr.h"
#include "../../base/log.h"
#include "../../net/systemmsg.h"
#include <stdio.h>

using namespace net;

int main()
{
    CClientSocketMgr csm;
    csm.Init(true, true);
    csm.Connect("127.0.0.1", 8888);
    csm.Run();

    MSG_BASE msg1;
    msg1.nMsg = 1111;
    csm.SendMsg(msg1);
    /*
    int nNum = 0;
    while (nNum < 2)
    {
        if (!csm.SendMsg(initmsg))
        {
            WriteLog("Send failed.\n");
        }
        nNum++;
    }
    */
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
            //MSG_LOGIN_LoginRequest msg;
            //csm.SendMsg(msg);
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
        if (strInput == "wd")
        {
    MSG_BASE msg;
    msg.nMsg = 1113;
    csm.SendMsg(msg);
        }
        else if (strInput =="d")
        {
    MSG_BASE msg;
    msg.nMsg = 1114;
    csm.SendMsg(msg);
        }
        else if (strInput =="ds")
        {
    MSG_BASE msg;
    msg.nMsg = 1115;
    csm.SendMsg(msg);
        }
        else if (strInput =="s")
        {
    MSG_BASE msg;
    msg.nMsg = 1116;
    csm.SendMsg(msg);
        }
        else if (strInput=="as")
        {
    MSG_BASE msg;
    msg.nMsg = 1117;
    csm.SendMsg(msg);
        }
        else if (strInput=="a")
        {
    MSG_BASE msg;
    msg.nMsg = 1118;
    csm.SendMsg(msg);
        }
        else if (strInput=="aw")
        {
    MSG_BASE msg;
    msg.nMsg = 1119;
    csm.SendMsg(msg);
        }
        else if (strInput=="w")
        {
    MSG_BASE msg;
    msg.nMsg = 1120;
    csm.SendMsg(msg);
        }
        else if (strInput=="p")
        {
    MSG_BASE msg;
    msg.nMsg = 1121;
    csm.SendMsg(msg);
        }
        else if (strInput =="q")
        {
            break;
        }
    }
    MSG_BASE msg2;
    msg2.nMsg = 1112;
    csm.SendMsg(msg2);
    csm.Disconnect();
    csm.Wait();
    return 0;
}

