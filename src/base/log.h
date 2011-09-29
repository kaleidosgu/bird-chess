#ifndef __base_log_h_
#define __base_log_h_

#include <stdio.h>
#include <time.h>
#include "mutex.h"

#define LOGMODE_STDOUT		0x0001
#define LOGMODE_STDERR		0x0002
#define LOGMODE_FILE		0x0004
#define LOGMODE_SYSLOG		0x0008
#define LOGMODE_YEAR		0x0100
#define LOGMODE_CR		0x0200
#define LOGMODE_TRUNCATE	0x1000

#ifndef MAX_PATH
#define MAX_PATH		260
#endif

namespace base
{
    enum LOGLEVEL
    {
        LEVEL_DEBUG,
        LEVEL_INFO,
        LEVEL_WARNING,
        LEVEL_ERROR,
        LEVEL_ALARM,
        LEVEL_SYSTEM,
    };
    static const unsigned int Max_Log_Buff_Length = 40960;

    class Logger
    {
        public:
            Logger();
            ~Logger();
            int SetLogMode(int nLogMode = LOGMODE_STDOUT, const char *szFileName = NULL);
            void Write(const char *lpszFmt, ...);
            void Write(int nLevel, const char *lpszFmt, ...);
            void SetLogLevel(int nLevel);
            void SetPerHour(bool bPerHour = false);
        private:
            void _Write(int nLogLevel, int nCurday, int nHour);
        protected:
            int m_nMode;
            int m_nLevel;
            bool m_bPerHour;
            char m_szFileName[MAX_PATH];
            char m_szBuff[Max_Log_Buff_Length];
            tm tmLast;
            int m_nFile;
            Mutex m_Mutex;
    };

    extern Logger g_loggerDefault;
    extern Logger g_loggerCore;

}
#define WriteLog base::g_loggerDefault.Write
#define WriteLogCore base::g_loggerCore.Write

#endif  // __base_log_h_

