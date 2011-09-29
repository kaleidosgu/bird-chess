#include "dbconn.h"
#include "dbmysql.h"
#include "../base/log.h"

using namespace base;

namespace db
{
    Logger g_loggerDBQuery;
    string DBServerData::s_strDefaultIP = "127.0.0.1";
    int DBServerData::s_nDefaultPort = 3306;
    string DBServerData::s_strDefaultUsername = "root";
    string DBServerData::s_strDefaultPassword = "";

    DBServerData::DBServerData(
            const char *szIP,
            int nPort,
            const char *szDBName,
            const char *szUsername,
            const char *szPassword)
    {
        m_strIP = (szIP == NULL ? s_strDefaultIP : szIP);
        m_nPort = (nPort > 0 ? nPort : s_nDefaultPort);
        m_strUsername = (szUsername == NULL ? s_strDefaultUsername : szUsername);
        m_strPassword = (szPassword == NULL ? s_strDefaultPassword : szPassword);
        m_strDBName = (szDBName == NULL ? "" : szDBName);
    }

    int DBServerData::GetPort() const
    {
        return m_nPort > 0 ? m_nPort : s_nDefaultPort;
    }
    const string & DBServerData::GetUserName() const
    {
        return m_strUsername.empty() ? s_strDefaultUsername : m_strUsername;
    }
    const string & DBServerData::GetPassword() const
    {
        return m_strPassword.empty() ? s_strDefaultPassword : m_strPassword;
    }

    unsigned int DBConn::s_nDefaultConnectTimeout = 1000;
    unsigned int DBConn::s_nDefaultReadTimeout = 1000;

    DBConn::DBConn(const char *szIP, int nPort, const char *szDBName, const char *szUsername, const char *szPassword) :
        m_DBServerData(szIP, nPort, szDBName, szUsername, szPassword)

    {
        m_pConn = NULL;
        m_nConnectTimeout = s_nDefaultConnectTimeout;
        m_nReadTimeout = s_nDefaultReadTimeout;
        m_bIsAutoCommit = true;
    }

    DBConn::~DBConn()
    {
        Close();
    }

    bool DBConn::Open(int nConnectTimeout /* = -1 */ , int nReadTimeout /* = -1 */ )
    {
        if (IsOpened())
        {
            WriteLog(LEVEL_WARNING, "The db connection has been opened. IP=%s, DBName=%s.\n", m_DBServerData.GetIP().c_str(), m_DBServerData.GetDBName().c_str());
            Close();
        }

        if (nConnectTimeout <= 0)
        {
            nConnectTimeout = s_nDefaultConnectTimeout;
        }
        if (nReadTimeout <= 0)
        {
            nReadTimeout = s_nDefaultReadTimeout;
        }

        m_pConn = mysql_init(NULL);
        MySQLUtil::set_mysql_timeout(m_pConn, MySQLUtil::ConnectTimeout, nConnectTimeout);
        MySQLUtil::set_mysql_timeout(m_pConn, MySQLUtil::ReadTimeout, nReadTimeout);
        MYSQL * ret = mysql_real_connect(
                m_pConn,
                m_DBServerData.GetIP().c_str(),
                m_DBServerData.GetUserName().c_str(),
                m_DBServerData.GetPassword().c_str(),
                m_DBServerData.GetDBName().c_str(),
                m_DBServerData.GetPort(),
                NULL,
                0);
        if (!ret)
        {
            int nErrorCode = mysql_errno(m_pConn);
            const char * szErrorMsg = mysql_error(m_pConn);
            mysql_close(m_pConn);
            m_pConn = NULL;
            WriteLog(LEVEL_ERROR,"Failed to connect to database. IP=%s, Port=%d, DBName=%s, ErrorCode=%d, ErrorMsg=%s.\n",
                    m_DBServerData.GetIP().c_str(),
                    m_DBServerData.GetPort(),
                    m_DBServerData.GetDBName().c_str(),
                    nErrorCode,
                    szErrorMsg);
            return false;
        }
        
        WriteLog("Connect to the database success. IP=%s, Port=%d, DBName=%s.\n",
                    m_DBServerData.GetIP().c_str(),
                    m_DBServerData.GetPort(),
                    m_DBServerData.GetDBName().c_str());

        m_nConnectTimeout = nConnectTimeout;
        m_nReadTimeout = nReadTimeout;
        mysql_autocommit(m_pConn, true);
        m_bIsAutoCommit = true;
        return true;
    }

    void DBConn::Close()
    {
        if (IsOpened())
        {
            mysql_close(m_pConn);
            //WriteLog(LEVEL_DEBUG, "Close DB Connection.\n");
            m_pConn = NULL;
        }
    }

    void DBConn::EscapeString(const char *s, string & out)
    {
        EscapeString(s, strlen(s), out);
    }

    void DBConn::EscapeString(const char *s, int nLen, string & out)
    {
        //ASSERT(s);
        //ASSERT(IsOpened());

        if (nLen)
        {
            char *buffer = (char *)malloc(nLen * 2 + 1);
            mysql_real_escape_string(m_pConn, buffer, s, nLen);
            out = buffer;
            free(buffer);
        }
    }

    int DBConn::Execute(const string & rstrSQL, DBDataSet * pDataSet /* = NULL */ , bool bRetryQueryOnFail /* = true */ )
    {
        return Execute(rstrSQL.c_str(), rstrSQL.length(), pDataSet, bRetryQueryOnFail);
    }

    int DBConn::Execute(const char *szSQL, unsigned int nSQLLen, DBDataSet * pDataSet /* = NULL */ , bool bRetryQueryOnFail /* = true */ )
    {
        //ASSERT(sql && *sql);
        //ASSERT(IsOpened());
        {
            int nRet = mysql_real_query(m_pConn, szSQL, nSQLLen);
            g_loggerDBQuery.Write(LEVEL_DEBUG, "Query: (%s).\n", szSQL);
            if (nRet != 0)
            {
                if (bRetryQueryOnFail)
                {
                    Open(m_nConnectTimeout, m_nReadTimeout);
                    nRet = mysql_real_query(m_pConn, szSQL, nSQLLen);
                }
                if (nRet != 0)
                {
                    int nErrorCode = mysql_errno(m_pConn);
                    const char * szErrorMsg = mysql_error(m_pConn);
                    g_loggerDBQuery.Write(LEVEL_ERROR, "Query Failed. SQL:'%s', ErrorCode=%d, ErrorMsg=%s.\n", szSQL, nErrorCode, szErrorMsg);
                    return -1;
                }
            }
        }

        MYSQL_RES * pRes = mysql_store_result(m_pConn);
        if (!pRes)
        {
            int nErrorCode = mysql_errno(m_pConn);
            if (nErrorCode != 0)
            {
                const char * szErrorMsg = mysql_error(m_pConn);
                g_loggerDBQuery.Write(LEVEL_ERROR, "Store Result Failed. SQL:'%s', ErrorCode=%d, ErrorMsg=%s.\n", szSQL, szErrorMsg, nErrorCode);
                return -2;
            }
        }

        int nAffected = mysql_affected_rows(m_pConn);
        if (pDataSet)
        {
            pDataSet->AddResult(m_pConn, pRes);
        }
        else
        {
            mysql_free_result(pRes);
        }
        return nAffected;
    }

    int DBConn::GetLastInsertId()
    {
        //ASSERT(IsOpened());
        return mysql_insert_id(m_pConn);
    }

    bool DBConn::_SetAutoCommit(bool bIsAutoCommit)
    {
        if (m_bIsAutoCommit != bIsAutoCommit)
        {
            m_bIsAutoCommit = bIsAutoCommit;
            mysql_autocommit(m_pConn, bIsAutoCommit);
            return true;
        }
        else
        {
            WriteLog(LEVEL_WARNING, "The src auto commit mode is same as the des auto commit mode. Commit Mode is %s.\n", bIsAutoCommit ? "Auto" : " Not Auto" );
            return false;
        }
    }
    
    void DBConn::BeginCommit()
    {
        _SetAutoCommit(false);
    }

    void DBConn::Commit()
    {
        if (!m_bIsAutoCommit)
        {
            mysql_commit(m_pConn);
            _SetAutoCommit(true);
        }
    }

    void DBConn::Rollback()
    {
        if (!m_bIsAutoCommit)
        {
            mysql_rollback(m_pConn);
            _SetAutoCommit(true);
        }
    }
}
