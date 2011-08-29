#include "dbquerytask.h"
#include "../base/log.h"

using namespace base;

namespace db
{
    DBQueryTask::DBQueryTask(int nDBQueryTaskType) :
        m_nDBQueryTaskType(nDBQueryTaskType)
    {
        m_bIsAtomic = false;
    }

    DBQueryTask::~DBQueryTask()
    {
        //WriteLog("DBQueryTask::~DBQueryTask, listsize=%d.\n",m_DBQueryList.size());
        _ClearDBQueryList();
    }

    bool DBQueryTask::AddDBQuery(DBQuery * pDBQuery)
    {
        if (pDBQuery == NULL)
        {
            WriteLog(LEVEL_WARNING, "Why Add NULL DBQuery?\n");
            return false;
        }
        else
        {
            m_DBQueryList.push_back(pDBQuery);
            return true;
        }
    }

    bool DBQueryTask::AddDBQuery(int nDBID, const char *szSQL)
    {
        DBQuery * pDBQuery = new DBQuery(nDBID, szSQL);
        m_DBQueryList.push_back(pDBQuery);
        return true;
    }

    void DBQueryTask::_ClearDBQueryList()
    {
        list< DBQuery * >::iterator it = m_DBQueryList.begin();
        for (; it!=m_DBQueryList.end(); ++it)
        {
            DBQuery * pDBQuery = *it;
            delete pDBQuery;
            pDBQuery = NULL;
            (*it) = NULL;
        }
        m_DBQueryList.clear();
    }

    void DBQueryTask::EnableAtomic()
    {
        m_bIsAtomic = true;
    }

    bool DBQueryTask::IsAtomic()
    {
        return m_bIsAtomic;
    }
    const list<DBQuery * > & DBQueryTask::GetDBQueryList()
    {
        return m_DBQueryList;
    }
}

