#ifndef __cardgame_dbquery_createplayerdbquery_h_
#define __cardgame_dbquery_createplayerdbquery_h_

#include "../../db/dbquery.h"
#include "../../base/log.h"

using namespace db; 
using namespace base;

class CreatePlayerDBQuery: public DBQuery
{
public:
    CreatePlayerDBQuery();
    virtual ~CreatePlayerDBQuery();

    virtual string BuildSQL();
    virtual void DisposeQueryResult(DBDataSet & rDataSet);

    string m_strUserName;
    string m_strPassword;
};

#endif // __cardgame_dbquery_createplayerdbquery_h_
