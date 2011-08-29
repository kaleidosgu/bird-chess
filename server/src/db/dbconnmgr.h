#ifndef __db_dbconnmgr_h_
#define __db_dbconnmgr_h_

#include "dbconn.h"
#include <string>

using namespace std;

namespace db
{
    class DBConnMgr
    {
    public:
        DBConnMgr();
        virtual ~DBConnMgr();
        bool AddDBConnection(int nDBID, const char *szIP, int nPort, const char *szDBName, const char *szUsername, const char *szPassword);
        DBConn * GetDBConnection(int nDBID);
        void ClearDBConnections();
    private:
        map< int, DBConn * > m_DBConnMap;
    };
}

#endif // __db_dbconnmgr_h_

