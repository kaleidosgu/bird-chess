#ifndef __cardgame_dbquery_logindbquery_h_
#define __cardgame_dbquery_logindbquery_h_

#include "../../db/dbquery.h"
#include "../../base/log.h"

using namespace db; 
using namespace base;

class LoginDBQuery: public DBQuery
{
public:
    LoginDBQuery();
    virtual ~LoginDBQuery();

    virtual string BuildSQL();
    virtual void DisposeQueryResult(DBDataSet & rDataSet);

    bool IsValidUser();
    bool IsUserExist();

    string m_strUserName;
    string m_strPassword;

    int m_nDBUserID;
    string m_strDBUserName;
    string m_strDBPassword;
    int m_nDBScore;
    int m_nDBWin;
    int m_nDBDogfall;
    int m_nDBLose;
private:
    bool m_bIsUserExist;
};

#endif // __cardgame_dbquery_logindbquery_h_
