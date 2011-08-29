#include "console.h"
#include "../base/thread.h"
#include "../base/log.h"
#include "../base/util.h"
#include <stdio.h>

const unsigned int cMAX_INPUT_CONSOLE_LEN = 1024;

using namespace base;

namespace core
{
    Console::Console(bool & rbRunning) :
        m_rbRunning(rbRunning)
    {
    }

    Console::~Console()
    {
    }

    void Console::Run()
    {
        char szInput[cMAX_INPUT_CONSOLE_LEN];
        while(m_rbRunning)
        {
            _GetInput(szInput);
            int nCommand = _ParseCommand(szInput);
            if (nCommand == 0)
            {
                m_rbRunning = false;
            }
            else if (nCommand > 0)
            {
                _ExecuteCommand(nCommand);
            }
            else
            {
                WriteLog(LEVEL_DEBUG, "Unknown Command: (%s).\n", szInput);
            }
        }
    }

    void Console::_GetInput(char *szInput)
    {
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
    }

    int Console::_ParseCommand(char * szInput)
    {
        string strCommand = szInput;
        base::util::TrimLeft(strCommand);
        base::util::TrimRight(strCommand);
        strCommand = base::util::ToLower(strCommand);
        if (strCommand == "quit" || strCommand == "exit")
        {
            return 0;
        }
        else
        {
            return -1;
        }
    }

    void Console::_ExecuteCommand(const int nCommand)
    {
    }
}
