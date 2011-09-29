#ifndef __db_dbquerytask_h_
#define __db_dbquerytask_h_

#include "dbquery.h"
#include "../core/task.h"

using namespace core;

namespace db
{
    class DBQueryWorker;
    class DBQueryTask : public Task
    {
        friend class DBQueryWorker;
    public:
        DBQueryTask(int nDBQueryTaskType);
        virtual ~DBQueryTask();

        bool AddDBQuery(DBQuery * pDBQuery);
        bool AddDBQuery(int nDBID, const char *szSQL);

        void EnableAtomic();
        bool IsAtomic();

        const int m_nDBQueryTaskType;
        const list<DBQuery * > & GetDBQueryList();
    private:
        void _ClearDBQueryList();
        list< DBQuery * > m_DBQueryList;
        bool m_bIsAtomic;
    };
}
#endif // __db_dbquerytask_h_
