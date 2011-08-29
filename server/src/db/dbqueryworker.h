#ifndef __db_dbqueryworker_h_
#define __db_dbqueryworker_h_

#include "../core/worker.h"
#include "dbquerytask.h"
#include "dbconnmgr.h"
#include <set>

using namespace std;
using namespace core;

namespace db
{
    enum DBID
    {
        DBID_GameDB,
    };
    class DBQueryWorker : public Worker
    {
    public:
        DBQueryWorker();
        virtual ~DBQueryWorker();
        bool Init();
    protected:
        virtual void DoTask(Task & rTask);
        void _BeginTransaction(DBConn * pDBConn);
        void _EndTransaction(set< DBConn * > & rDBConnSet, bool bResult);
        bool _AddDBConnection(int nDBID, const char *szDBID);

        DBConnMgr m_DBConnMgr;
    };
}

#endif // __db_dbqueryworker_h_
