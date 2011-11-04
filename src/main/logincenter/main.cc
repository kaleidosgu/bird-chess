#include "../../base/log.h"
#include "../../base/assert.h"
#include "../../login/logincenter.h"

using namespace base;

CLoginCenter g_LoginCenter;

int main()
{
    g_iniDefault.Load("./config/server.conf");
    WriteLog(LEVEL_DEBUG, "Start Login Center.\n");
    g_LoginCenter.Start();
    WriteLog(LEVEL_DEBUG, "Exit Login Center.\n");
    return 0;
}
