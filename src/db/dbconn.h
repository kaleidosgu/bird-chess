#ifndef __db_dbconn_h_
#define __db_dbconn_h_

#include "dbdataset.h"
#include "../base/log.h"

#include <string>
#include <map>
#include <vector>

using namespace std;

namespace db
{
    class DBServerData
    {
    public:
        static string s_strDefaultIP;
        static int s_nDefaultPort;
        static string s_strDefaultUsername;
        static string s_strDefaultPassword;

    public:

        DBServerData(const char *szIP, int nPort, const char *szDBName, const char *szUsername, const char *szPassword);

        const string & GetIP() const
        {
            return m_strIP;
        }
        int GetPort() const;
        const string & GetUserName() const;
        const string & GetPassword() const;
        const string & GetDBName() const
        {
            return m_strDBName;
        }
    private:
        string m_strIP;
        int m_nPort;
        string m_strUsername;
        string m_strPassword;
        string m_strDBName;
    };

    /**
     * A connection class that connects to any of our databases.
     */
    class DBConn
    {
    public:
        //static unsigned int s_nDefaultWorkerCount;	// for parallel
        // executions
        static unsigned int s_nDefaultConnectTimeout;
        static unsigned int s_nDefaultReadTimeout;

    public:
        DBConn(const char *szIP, int nPort, const char *szDBName, const char *szUsername, const char *szPassword);
        ~DBConn();

        /**
         * Open a database by specifying a type and an id.
         */
        bool Open(int nConnectTimeout = -1, int nReadTimeout = -1);

        /**
         * Run an SQL and return number of affected rows. Consider DBQuery class,
         * instead of directly calling this function.
         */
        int Execute(const string & rstrSQL, DBDataSet * pDateSet = NULL, bool bRetryQueryOnFail = true);
        int Execute(const char *szSQL, unsigned int nSQLLen, DBDataSet * pDateSet = NULL, bool bRetryQueryOnFail = true);

        /**
         * Returns newly created id from most recent insertion.
         */
        int GetLastInsertId();

        void Close();

        bool IsOpened() const
        {
            return m_pConn != NULL;
        }
        /**
         * Helper function for escaping strings in SQLs.
         */
        void EscapeString(const char *s, string & out);
        void EscapeString(const char *s, int nLen, string & out);

        void BeginCommit();
        void Commit();
        void Rollback();

    private:
        bool _SetAutoCommit(bool bIsAutoCommit);

        bool m_bIsAutoCommit;
        DBServerData m_DBServerData;
        MYSQL * m_pConn;
        unsigned int m_nConnectTimeout;
        unsigned int m_nReadTimeout;
    };

    extern base::Logger g_loggerDBQuery;
}

#endif // __db_dbconn_h_
