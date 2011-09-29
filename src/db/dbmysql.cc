#include "dbmysql.h"
#include "../base/log.h"
#include <mysql/mysql.h>

const unsigned int cMAX_ESCAPESTRING_LEN = 2000;

using namespace base;

namespace db
{
    int MySQLUtil::set_mysql_timeout(MYSQL * mysql, MySQLUtil::TimeoutType type, int ms)
    {
        mysql_option opt = MYSQL_OPT_CONNECT_TIMEOUT;
#ifdef MYSQL_MILLISECOND_TIMEOUT
        switch (type)
        {
            case MySQLUtil::ConnectTimeout:
                opt = MYSQL_OPT_CONNECT_TIMEOUT_MS;
                break;
            case MySQLUtil::ReadTimeout:
                opt = MYSQL_OPT_READ_TIMEOUT_MS;
                break;
            case MySQLUtil::WriteTimeout:
                opt = MYSQL_OPT_WRITE_TIMEOUT_MS;
                break;
            default:
                break;
        }
#else
        switch (type)
        {
            case MySQLUtil::ConnectTimeout:
                opt = MYSQL_OPT_CONNECT_TIMEOUT;
                break;
            case MySQLUtil::ReadTimeout:
                opt = MYSQL_OPT_READ_TIMEOUT;
                break;
            case MySQLUtil::WriteTimeout:
                opt = MYSQL_OPT_WRITE_TIMEOUT;
                break;
            default:
                break;
        }
        ms = (ms + 999) / 1000;
#endif

        return mysql_options(mysql, opt, (const char *)&ms);
    }
    bool MySQLUtil::EscapeString(const char *szSrc, string & rstrDes)
    {
        return EscapeString(szSrc, strlen(szSrc), rstrDes);
    }

    bool MySQLUtil::EscapeString(const char *szSrc, size_t nLen, string & rstrDes)
    {
        bool bRes = false;
        if (nLen == 0)
        {
            rstrDes = "";
            bRes = true;
        }
        else if (nLen * 2 >= cMAX_ESCAPESTRING_LEN)
        {
            WriteLog(LEVEL_ERROR, "MySQLUtil::EscapeString. The src len is too long. Len=%d.\n", nLen);
            bRes = false;
        }
        else
        {
            char szBuff[cMAX_ESCAPESTRING_LEN];
            mysql_escape_string(szBuff, szSrc, nLen);
            rstrDes = szBuff;
            bRes = true;
        }
        return bRes;
    }
}
