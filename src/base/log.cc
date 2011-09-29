#include "log.h"
#include <stdarg.h>
#include <string>

#include <syslog.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

namespace base
{
    Logger g_loggerDefault;
    Logger g_loggerCore;
    static char level_str[][10] =
    { "DEBUG", "INFO", "WARNING", "ERROR", "ALARM", "SYSTEM" };

    Logger::Logger()
    {
        m_nLevel = 0;
        m_bPerHour = false;
        m_nFile = 0;
        SetLogMode();
    }

    Logger::~Logger()
    {
        if (m_nFile)
        {
            close(m_nFile);
            m_nFile = 0;
        }
    }

    void Logger::SetPerHour(bool bPerHour)
    {
        m_bPerHour = bPerHour;
    }

    void Logger::SetLogLevel(int nLevel)
    {
        m_nLevel = nLevel;
    }

    int Logger::SetLogMode(int nLogMode, const char *szFileName)
    {
        m_nMode = nLogMode;
        if (m_nMode & LOGMODE_FILE)
        {
            const size_t nMax_File_Name = 150;
            time_t now = time(NULL);
            tm *pNow;
            tm tmNow;
            localtime_r(&now, &tmNow);
            pNow = &tmNow;
            tmLast.tm_mday = pNow->tm_mday;
            tmLast.tm_hour = pNow->tm_hour;
            char szBuff[MAX_PATH];
            if (szFileName == NULL || strlen(szFileName) == 0 || strlen(szFileName) > nMax_File_Name)
            {
                strcpy(m_szFileName, "./log");
            }
            else
            {
                strcpy(m_szFileName, szFileName);
            }
            if (m_nMode & LOGMODE_TRUNCATE)
            {
                if (m_bPerHour)
                {
                    if (m_nMode & LOGMODE_YEAR)
                    {
                        sprintf(szBuff,
                                "%s.%04d-%02d-%02d-%02d.log",
                                m_szFileName,
                                1900 + pNow->tm_year,
                                1 + pNow->tm_mon,
                                pNow->tm_mday,
                                pNow->tm_hour);
                    }
                    else
                    {
                        sprintf(szBuff,
                                "%s.%02d-%02d-%02d.log",
                                m_szFileName,
                                1 + pNow->tm_mon,
                                pNow->tm_mday,
                                pNow->tm_hour);
                    }
                }
                else
                {
                    if (m_nMode & LOGMODE_YEAR)
                    {
                        sprintf(szBuff,
                                "%s.%04d-%02d-%02d.log",
                                m_szFileName,
                                1900 + pNow->tm_year,
                                1 + pNow->tm_mon,
                                pNow->tm_mday);
                    }
                    else
                    {
                        sprintf(szBuff,
                                "%s.%02d-%02d.log",
                                m_szFileName,
                                1 + pNow->tm_mon,
                                pNow->tm_mday);
                    }
                }
            }
            else
            {
                strcpy(szBuff, m_szFileName);
            }
            if (m_nFile)
            {
                close(m_nFile);
                m_nFile = 0;
            }
            m_nFile = open(szBuff, O_WRONLY | O_CREAT | O_LARGEFILE | O_APPEND, 0600);
            if (m_nFile == -1)
            {
                m_nMode &= ~LOGMODE_FILE;
            }
        }
        return m_nMode;
    }

    void Logger::_Write(int nLogLevel, int nCurday, int nCurHour)
    {
        char *ptrEnd = m_szBuff + strlen(m_szBuff) - 1;
        if (*ptrEnd == 0x0A)
        {
            *ptrEnd++ = ']';
            *ptrEnd++ = 0x0D;
            *ptrEnd++ = 0x0A;
            *ptrEnd++ = 0;
        }
        else
        {
            *++ptrEnd = ']';
            *++ptrEnd = 0;
        }
        if (m_nMode & LOGMODE_CR)
        {
            ptrEnd = m_szBuff + strlen(m_szBuff) - 1;
            if (*ptrEnd != 0x0A)
            {
                ptrEnd++;
                *ptrEnd++ = 0x0D;
                *ptrEnd++ = 0x0A;
                *ptrEnd++ = 0;
            }
        }
        if (m_nMode & LOGMODE_STDOUT)
        {
            fwrite(m_szBuff, strlen(m_szBuff), 1, stdout);
            fflush(stdout);
        }
        if (m_nMode & LOGMODE_STDERR)
        {
            fwrite(m_szBuff, strlen(m_szBuff), 1, stderr);
            fflush(stderr);
        }
        if ((m_nMode & LOGMODE_FILE) && (m_nMode & LOGMODE_TRUNCATE))
        {
            if (m_bPerHour && tmLast.tm_hour != nCurHour)
            {
                close(m_nFile);
                m_nFile = 0;
                SetLogMode(m_nMode, m_szFileName);
            }
            else if (tmLast.tm_mday != nCurday)
            {
                close(m_nFile);
                m_nFile = 0;
                SetLogMode(m_nMode, m_szFileName);
            }
        }
        if ((m_nMode & LOGMODE_FILE) && (nLogLevel >= m_nLevel))
        {
            write(m_nFile, m_szBuff, strlen(m_szBuff));
        }
        if (m_nMode & LOGMODE_SYSLOG)
        {
            //syslog();
        }
    }

    void Logger::Write(const char *lpszFmt, ...)
    {
        int nLogLevel = LEVEL_INFO;
        if (m_nMode == 0)
        {
            return;
        }
        Mutex::ScopedLock lock(m_Mutex);
        if (lpszFmt)
        {
            time_t now = time(NULL);
            tm *pNow;
            tm tmNow;
            localtime_r(&now, &tmNow);
            pNow = &tmNow;
            if (m_nMode & LOGMODE_YEAR)
            {
                sprintf(m_szBuff,
                        "[%04d-%02d-%02d %02d:%02d:%02d][%s][",
                        1900 + pNow->tm_year,
                        1 + pNow->tm_mon,
                        pNow->tm_mday,
                        pNow->tm_hour,
                        pNow->tm_min,
                        pNow->tm_sec,
                        level_str[nLogLevel]);
            }
            else
            {
                sprintf(m_szBuff, "[%02d-%02d %02d:%02d:%02d][%s][",
                        1 + pNow->tm_mon,
                        pNow->tm_mday,
                        pNow->tm_hour,
                        pNow->tm_min,
                        pNow->tm_sec,
                        level_str[nLogLevel]);
            }
            size_t nLen = strlen(m_szBuff);
            va_list argList;
            va_start(argList, lpszFmt);
            vsnprintf(m_szBuff + nLen, Max_Log_Buff_Length - 3 - nLen, lpszFmt, argList);
            va_end(argList);
            _Write(nLogLevel, pNow->tm_mday, pNow->tm_hour);
        }
    }

    void Logger::Write(int nLogLevel, const char *lpszFmt, ...)
    {
        if (m_nMode == 0)
        {
            return;
        }
        Mutex::ScopedLock lock(m_Mutex);
        if (lpszFmt)
        {
            time_t now = time(NULL);
            tm *pNow;
            tm tmNow;
            localtime_r(&now, &tmNow);
            pNow = &tmNow;
            if (m_nMode & LOGMODE_YEAR)
            {
                sprintf(m_szBuff,
                        "[%04d-%02d-%02d %02d:%02d:%02d][%s][",
                        1900 + pNow->tm_year,
                        1 + pNow->tm_mon,
                        pNow->tm_mday,
                        pNow->tm_hour,
                        pNow->tm_min,
                        pNow->tm_sec,
                        level_str[nLogLevel]);
            }
            else
            {
                sprintf(m_szBuff, "[%02d-%02d %02d:%02d:%02d][%s][",
                        1 + pNow->tm_mon,
                        pNow->tm_mday,
                        pNow->tm_hour,
                        pNow->tm_min,
                        pNow->tm_sec,
                        level_str[nLogLevel]);
            }
            size_t nLen = strlen(m_szBuff);
            va_list argList;
            va_start(argList, lpszFmt);
            vsnprintf(m_szBuff + nLen, Max_Log_Buff_Length - 3 - nLen, lpszFmt, argList);
            va_end(argList);
            _Write(nLogLevel, pNow->tm_mday, pNow->tm_hour);
        }
    }
}

