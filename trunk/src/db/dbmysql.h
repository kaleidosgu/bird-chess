#ifndef __db_dbmysql_h_
#define __db_dbmysql_h_

#include <mysql/mysql.h>
#include <string>

using namespace std;

namespace db
{
    class MySQLUtil
    {
    public:
        enum TimeoutType
        {
            ConnectTimeout,
            ReadTimeout,
            WriteTimeout
        };

        static int set_mysql_timeout(MYSQL * mysql, MySQLUtil::TimeoutType type, int ms);

        static bool EscapeString(const char *szSrc, string & rstrDes);

        static bool EscapeString(const char *szSrc, size_t nLen, string & rstrDes);
    };
}
#endif // __db_dbmysql_h_
