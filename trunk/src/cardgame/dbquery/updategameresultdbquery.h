#ifndef __cardgame_dbquery_updategameresultdbquery_h_
#define __cardgame_dbquery_updategameresultdbquery_h_

#include "../../db/dbquery.h"
#include "../../base/log.h"

using namespace db; 
using namespace base;

class UpdateGameResultDBQuery: public DBQuery
{
public:
    enum GameResult
    {
        GameResult_Lose = -1,
        GameResult_Dogfall = 0,
        GameResult_Win = 1
    };
    UpdateGameResultDBQuery();
    virtual ~UpdateGameResultDBQuery();

    virtual string BuildSQL();
    virtual void DisposeQueryResult(DBDataSet & rDataSet);
    int m_nPlayerID;
    int m_nScore;
    GameResult m_GameResult;
};

#endif // __cardgame_dbquery_updategameresultdbquery_h_
