#include "dbqueryworker.h"
#include "../base/log.h"
#include "../base/ini.h"

using namespace base;

namespace db
{
    DBQueryWorker::DBQueryWorker()
    {
    }
    DBQueryWorker::~DBQueryWorker()
    {
    }

    bool DBQueryWorker::Init()
    {
        bool bRes = true;
        bRes = _AddDBConnection(DBID_GameDB, "GameDB");
        return bRes;
    }

    bool DBQueryWorker::_AddDBConnection(int nDBID, const char *szDBID)
    {
        string strIP = "127.0.0.1";
        int nPort = 3306;
        string strUsername = "root";
        string strPassword = "";
        string strDBName = "";

        GIni.GetString(szDBID, "IP", strIP, "127.0.0.1");
        nPort = GIni.GetInt(szDBID, "Port", 3306);
        GIni.GetString(szDBID, "Username", strUsername, "root");
        GIni.GetString(szDBID, "Password", strPassword, "");
        GIni.GetString(szDBID, "DBName", strDBName, "");

        return m_DBConnMgr.AddDBConnection(nDBID, strIP.c_str(), nPort, strDBName.c_str(), strUsername.c_str(), strPassword.c_str());
    }

    void DBQueryWorker::_BeginTransaction(DBConn * pDBConn)
    {
        if (pDBConn)
        {
            pDBConn->BeginCommit();
        }
    }

    void DBQueryWorker::_EndTransaction(set < DBConn * > & rDBConnSet, bool bResult)
    {
        set< DBConn * >::iterator it = rDBConnSet.begin();
        for (; it!=rDBConnSet.end(); ++it)
        {
            DBConn * pDBConn = *it;
            if (pDBConn)
            {
                if (bResult)
                {
                    pDBConn->Commit();
                }
                else
                {
                    pDBConn->Rollback();
                }
            }
        }
    }

    void DBQueryWorker::DoTask(Task & rTask)
    {
        bool bRes = true;
        DBQueryTask & rDBQueryTask = (DBQueryTask &)rTask;
        //WriteLog(LEVEL_DEBUG, "I am doing the db query task. WorkerID = %d, TaskID=%d.\n", m_nWorkerID, rDBQueryTask.GetTaskID());
        rDBQueryTask.SetTaskState(Task::TaskState_Success);
        set < DBConn * > dbConnSet;
        list< DBQuery * >::iterator it = rDBQueryTask.m_DBQueryList.begin();
        for (; it!=rDBQueryTask.m_DBQueryList.end(); ++it)
        {
            DBQuery * pDBQuery = *it;
            if (pDBQuery != NULL)
            {
                DBConn * pDBConn = m_DBConnMgr.GetDBConnection(pDBQuery->GetDBID());
                if (pDBConn != NULL)
                {
                    if (rDBQueryTask.IsAtomic())
                    {
                        if (dbConnSet.find(pDBConn) == dbConnSet.end())
                        {
                            dbConnSet.insert(pDBConn);
                            _BeginTransaction(pDBConn);
                        }
                    }
                    pDBQuery->SetDBConn(pDBConn);
                    if (!pDBQuery->Execute())
                    {
                        rDBQueryTask.SetTaskState(Task::TaskState_Failed);
                        bRes = false;
                    }
                }
                else
                {
                    rDBQueryTask.SetTaskState(Task::TaskState_Error);
                }
            }
            else
            {
                WriteLog(LEVEL_ERROR, "Why does there have the NULL DBQuery. TaskID=%d.\n", rDBQueryTask.GetTaskID());
            }
        }
        if (rDBQueryTask.IsAtomic())
        {
            _EndTransaction(dbConnSet, bRes);
        }
    }
}
