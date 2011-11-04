#include "logindbquery.h"
#include "../../db/dbqueryworker.h"

LoginDBQuery::LoginDBQuery() :
    DBQuery(DBID_GameDB)
{
    m_strUserName = "";
    m_strPassword = "";

    m_nDBUserID = -1;
    m_strDBUserName = "";
    m_strDBPassword = "";
    m_nDBScore = 0;
    m_nDBWin = 0;
    m_nDBDogfall = 0;
    m_nDBLose = 0;
    m_bIsUserExist = false;
}

LoginDBQuery::~LoginDBQuery()
{
}

string LoginDBQuery::BuildSQL()
{
    string strUserName;
    MySQLUtil::EscapeString(m_strUserName.c_str(), strUserName);
    string szSQL =  "SELECT PlayerID, PlayerName, Password, Score, Win, Dogfall, Lose FROM player WHERE PlayerName='"; 
    szSQL += strUserName;
    szSQL += "'";
    return szSQL;
}

void LoginDBQuery::DisposeQueryResult(DBDataSet & rDataSet)
{
    if (rDataSet.GetRowCount() != 0)
    {
        m_bIsUserExist = true;
    }
    if (rDataSet.GetRowCount() == 1)
    {
        rDataSet.MoveFirst();
        rDataSet.GetRow();
        m_nDBUserID = rDataSet.GetIntField(0);
        m_strDBUserName = rDataSet.GetStringField(1);
        m_strDBPassword = rDataSet.GetStringField(2);
        m_nDBScore = rDataSet.GetIntField(3);
        m_nDBWin = rDataSet.GetIntField(4);
        m_nDBDogfall = rDataSet.GetIntField(5);
        m_nDBLose = rDataSet.GetIntField(6);
    }
    else
    {
        WriteLog(LEVEL_ERROR, "LoginDBQuery::DisposeQueryResult. GetRowCount(%d) Error!\n", rDataSet.GetRowCount());
    }
}

bool LoginDBQuery::IsValidUser()
{
    if (m_strDBPassword != "" && m_strDBPassword == m_strPassword)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool LoginDBQuery::IsUserExist()
{
    return m_bIsUserExist;
}
