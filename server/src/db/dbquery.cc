#include "dbquery.h"
#include "../base/log.h"

using namespace base;

namespace db
{
    DBQuery::DBQuery(int nDBID)
    {
        m_strSQL = "";
        m_nDBID = nDBID;
        m_pConn = NULL;
    }
    DBQuery::DBQuery(int nDBID, const char *szSQL)
    {
        m_strSQL = szSQL;
        m_nDBID = nDBID;
        m_pConn = NULL;
    }
    DBQuery::~DBQuery()
    {
    }

    bool DBQuery::Execute()
    {
        if (m_pConn == NULL)
        {
            WriteLog(LEVEL_ERROR, "You have not set the DBConnection. DBID=%d.\n", m_nDBID);
            return false;
        }
        DBDataSet dataSet;
        int nAffected = m_pConn->Execute(BuildSQL(), &dataSet);
        //int nAffected = m_pConn->Execute(BuildSQL());
        if (nAffected >=0)
        {
            DisposeQueryResult(dataSet);
            return true;
        }
        else
        {
            return false;
        }
    }

    string DBQuery::BuildSQL()
    {
        if (m_strSQL == "")
        {
            return "SELECT 1";
        }
        else
        {
            return m_strSQL;
        }
    }

    void DBQuery::DisposeQueryResult(DBDataSet & rDataSet)
    {
        for (rDataSet.MoveFirst(); rDataSet.GetRow(); rDataSet.MoveNext())
        {
            WriteLog(LEVEL_DEBUG, "DBQuery::DisposeQueryResult. Row string is (%s).\n", rDataSet.GetRowString().c_str());
        }
    }

    int DBQuery::GetDBID()
    {
        return m_nDBID;
    }

    void DBQuery::SetDBConn(DBConn * pConn)
    {
        m_pConn = pConn;
    }
}
