#include "createplayerdbquery.h"
#include "../../db/dbqueryworker.h"

CreatePlayerDBQuery::CreatePlayerDBQuery() :
    DBQuery(DBID_GameDB)
{
    m_strUserName = "";
    m_strPassword = "";
}

CreatePlayerDBQuery::~CreatePlayerDBQuery()
{
}

string CreatePlayerDBQuery::BuildSQL()
{
        string strUserName;
        MySQLUtil::EscapeString(m_strUserName.c_str(), strUserName);
        string strPassword;
        MySQLUtil::EscapeString(m_strPassword.c_str(), strPassword);
        
        char szSQL[1024];
        sprintf(szSQL, "INSERT INTO player (PlayerName, Password, Score, Win, Dogfall, Lose, RegisterTime) VALUES ('%s', '%s', 0, 0, 0, 0, %u)", strUserName.c_str(), strPassword.c_str(), (unsigned int)(time(NULL)));
        return szSQL;
}

void CreatePlayerDBQuery::DisposeQueryResult(DBDataSet & rDataSet)
{
}
