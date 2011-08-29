#include "dbconnmgr.h"
#include "../base/log.h"
#include "../base/thread.h"

using namespace base;

namespace db
{
    DBConnMgr::DBConnMgr()
    {
    }

    DBConnMgr::~DBConnMgr()
    {
        ClearDBConnections();
    }

    bool DBConnMgr::AddDBConnection(int nDBID, const char *szIP, int nPort, const char *szDBName, const char *szUsername, const char *szPassword)
    {
        map< int, DBConn * >::iterator it = m_DBConnMap.find(nDBID);
        if (it != m_DBConnMap.end())
        {
            WriteLog(LEVEL_WARNING, "This DBID has been added.DBID=%d, IP=%s, Port=%d, DBName=%s.\n", nDBID, szIP, nPort, szDBName);
            return false;
        }
        else
        {
            DBConn * pDBConn = new DBConn(szIP, nPort, szDBName, szUsername, szPassword);
            if (pDBConn->Open())
            {
                m_DBConnMap[nDBID] = pDBConn;
                return true;
            }
            else
            {
                return false;
            }
        }
    }

    void DBConnMgr::ClearDBConnections()
    {
        map< int, DBConn * >::iterator it = m_DBConnMap.begin();
        for (; it!=m_DBConnMap.end(); ++it)
        {
            DBConn * pDBConn = it->second;
            if (pDBConn)
            {
                pDBConn->Close();
                delete pDBConn;
                pDBConn = NULL;
            }
        }
        m_DBConnMap.clear();
    }
    
    DBConn * DBConnMgr::GetDBConnection(int nDBID)
    {
        DBConn * pDBConn = NULL;
        map< int, DBConn * >::iterator it = m_DBConnMap.find(nDBID);
        if (it != m_DBConnMap.end())
        {
            pDBConn = it->second;
        }
        else
        {
            WriteLog(LEVEL_WARNING, "Can't find the DB connection by the DBID. DBID=%d.\n", nDBID);
        }
        return pDBConn;
    }
}
