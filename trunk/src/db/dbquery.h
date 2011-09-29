#ifndef __db_dbquery_h_
#define __db_dbquery_h_

#include <string>
#include "dbconn.h"
#include "dbdataset.h"
#include "dbmysql.h"

using namespace std;

namespace db
{
    class DBQuery
    {
        friend class DBQueryTask;
    public:
        DBQuery(int nDBID);
        virtual ~DBQuery();

        bool Execute();
        int GetDBID();
        void SetDBConn(DBConn * pConn);
    protected:
        virtual string BuildSQL();
        virtual void DisposeQueryResult(DBDataSet & rDataSet);

    private:
        DBQuery(int nDBID, const char *szSQL);
        int m_nDBID;
        DBConn * m_pConn;
        string m_strSQL;
    };
}
#endif // __db_dbquery_h_
