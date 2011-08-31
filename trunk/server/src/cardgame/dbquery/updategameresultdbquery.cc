#include "updategameresultdbquery.h"
#include "../../db/dbqueryworker.h"

UpdateGameResultDBQuery::UpdateGameResultDBQuery() :
    DBQuery(DBID_GameDB)
{
    m_nPlayerID = -1;
    m_nScore = 0;
    m_GameResult = GameResult_Dogfall;
}

UpdateGameResultDBQuery::~UpdateGameResultDBQuery()
{
}

string UpdateGameResultDBQuery::BuildSQL()
{
    char szSQL[1024];
    sprintf(szSQL, "UPDATE player SET Score=Score+%d, Win=Win + %d, Dogfall=Dogfall+%d, Lose=Lose+%d WHERE PlayerID=%d", m_nScore, m_GameResult == GameResult_Win ? 1:0, m_GameResult == GameResult_Dogfall ? 1:0, m_GameResult == GameResult_Lose ? 1:0, m_nPlayerID);
    return szSQL;
}

void UpdateGameResultDBQuery::DisposeQueryResult(DBDataSet & rDataSet)
{
}
