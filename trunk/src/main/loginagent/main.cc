#include "../../base/log.h"
#include "../../base/assert.h"
#include "../../login/loginagent.h"

using namespace base;

CLoginAgent g_LoginAgent;

int main()
{
    g_iniDefault.Load("./config/server.conf");
    WriteLog(LEVEL_DEBUG, "Start Login Agent.\n");
    g_LoginAgent.Start();
    WriteLog(LEVEL_DEBUG, "Exit Login Agent.\n");
    return 0;
}
