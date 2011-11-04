#ifndef __cardgame_cardgame_h_
#define __cardgame_cardgame_h_

#include "../core/servercirculator.h"
#include "../db/dbqueryfactory.h"
#include "dbquery/logindbquery.h"
#include "roommgr.h"

using namespace core;
using namespace db;

class CardSlot
{
public:
    enum State
    {
        State_Free,
        State_Logining,
        State_InGame,
        State_InRoom,
    };
public:
    CardSlot()
    {
        m_nSlotIndex = -1;
        m_pPlayer = NULL;
        m_State = State_Free;
        m_nPage = 0;
    };
    ~CardSlot(){};
    int m_nSlotIndex;
    Player * m_pPlayer;
    State m_State;
    int m_nPage;
};

class CCardGame : public CServerCirculator
{
public:
    CCardGame();
    virtual ~CCardGame();
protected:
    virtual bool Initialize();
    virtual void Pump();
    virtual void ProcessMsg(MSG_BASE & rMsg, unsigned int nSlotIndex);
    virtual void HeartBeat(unsigned int nIntervalMS);
    virtual void Destory();
    void ProcessDBQueryResult(DBQueryTask & rDBQueryTask);

private:
    void _PlayerLogin(LoginDBQuery & rLoginDBQuery, CardSlot * pSlot);
    void _DisconnectClient(CardSlot * pSlot, int nReason);

    RoomMgr m_RoomMgr;
    unsigned int m_nMaxClient;
    CardSlot * m_aCardSlot;
    map < int, int > m_PlayerIDMapSlotIndex;
    map < long long, CardSlot *> m_TaskIDSlotMap;
};

#endif // __cardgame_cardgame_h_
