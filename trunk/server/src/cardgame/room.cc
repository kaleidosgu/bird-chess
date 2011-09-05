#include "room.h"
#include "../base/log.h"
#include "../db/dbqueryfactory.h"
#include "dbquery/updategameresultdbquery.h"
#include "dbquery/cardgamedbquerytasktype.h"

using namespace base;
using namespace db;

extern const CardList g_CardList;

static int s_nRoomID = 0;

Room::Room()
{
    m_nRoomID = s_nRoomID;
    s_nRoomID++;
    m_bGameStarted = false;
    m_bIsValid = true;

    m_pRoomMaster = NULL;
    for (unsigned int i=0; i<cMAX_USER_CARD_TYPE; i++)
    {
        m_aCardType[i] = (CardType)(CardType_Red + i);
        m_aCardTypePath[i] = 0;
    }
    for (unsigned int i=0; i<cMAX_PATH_LEN; i++)
    {
        m_Path[i].SetPathID(i);
    }

    for (unsigned int i=0; i<cMAX_CARD_LIST_NUM; i++)
    {
        m_aCardListIndex[i] = i;
    }
    m_nCurrentCardPos = 0;

    srand(time(NULL));
}

Room::~Room()
{
}

int Room::PlayerEnter(Player & rPlayer)
{
    if (!m_bIsValid)
    {
        return MSG_CARDGAME_S2C_EnterRoomResult::Result_RoomIsInvalid;
    }
    if (m_bGameStarted)
    {
        return MSG_CARDGAME_S2C_EnterRoomResult::Result_GameHasStarted;
    }

    if (m_PlayerMap.size() >=cMAX_PLAYER_NUM)
    {
        // the player number need less than 5
        return MSG_CARDGAME_S2C_EnterRoomResult::Result_RoomIsFull;
    }
    else
    {
        WriteLog(LEVEL_DEBUG, "A player(%d) Enter the room(%d).\n", rPlayer.GetPlayerID(), m_nRoomID);
        // Broadcast
        MSG_CARDGAME_S2C_PlayerEnter msg;
        msg.nPlayerID = rPlayer.GetPlayerID();
        strncpy(msg.szPlayerName, rPlayer.GetPlayerName().c_str(), cMAX_USERNAME_LEN - 1);
        msg.nScore = rPlayer.GetScore();
        msg.nWin = rPlayer.GetWin();
        msg.nDogfall = rPlayer.GetDogfall();
        msg.nLose = rPlayer.GetLose();
        _BroadCastMsg(msg);

        // first player
        if (m_pRoomMaster == NULL)
        {
            m_pRoomMaster = &rPlayer;
        }

        m_PlayerMap[rPlayer.GetPlayerID()] = &rPlayer;

        //upate playerlist to player
        _SendAllPlayerInfoToPlayer(rPlayer);

        if (m_pRoomMaster)
        {
            MSG_CARDGAME_S2C_RoomMaster msg;
            msg.nPlayerID = m_pRoomMaster->GetPlayerID();
            rPlayer.SendMsg(msg);
        }

        return MSG_CARDGAME_S2C_EnterRoomResult::Result_Success;
    }
}

void Room::_SendAllPlayerInfoToPlayer(Player & rPlayer)
{
    map< int, Player * >::iterator it = m_PlayerMap.begin();
    for (; it!=m_PlayerMap.end(); ++it)
    {
        Player * pPlayer = it->second;
        if (pPlayer)
        {
            MSG_CARDGAME_S2C_PlayerEnter msg;
            msg.nPlayerID = pPlayer->GetPlayerID();
            strncpy(msg.szPlayerName, pPlayer->GetPlayerName().c_str(), cMAX_USERNAME_LEN - 1);
            msg.nScore = pPlayer->GetScore();
            msg.nWin = pPlayer->GetWin();
            msg.nDogfall = pPlayer->GetDogfall();
            msg.nLose = pPlayer->GetLose();
            rPlayer.SendMsg(msg);
        }
    }
}

void Room::_BroadCastMsg(MSG_BASE & rMsg)
{
    map< int, Player * >::iterator it = m_PlayerMap.begin();
    for (; it!=m_PlayerMap.end(); ++it)
    {
        Player * pPlayer = it->second;
        if (pPlayer)
        {
            pPlayer->SendMsg(rMsg);
        }
    }
}

bool Room::PlayerLeave(Player & rPlayer)
{
    WriteLog(LEVEL_DEBUG, "Room::PlayerLeave. RoomID(%d).\n", m_nRoomID);
    // confirm the player is in the room
    map< int, Player * >::iterator it = m_PlayerMap.find(rPlayer.GetPlayerID());
    if (it == m_PlayerMap.end())
    {
        WriteLog(LEVEL_DEBUG, "Room::PlayerLeave. Can't find the player(%d). RoomID(%d).\n", rPlayer.GetPlayerID(), m_nRoomID);
        return false;
    }

    if (&rPlayer == m_pRoomMaster)
    {
        m_pRoomMaster = NULL;
    }
    bool bCurrentPlayer = false;
    if (m_bGameStarted && it== m_it)
    {
        bCurrentPlayer = true;
        _TurnToNextPlayer();
    }
    m_PlayerMap.erase(it);
    if (m_PlayerMap.empty())
    {
        // No player, and set room invalid
        m_bIsValid = false;
    }
    else
    {
        // Broadcast
        MSG_CARDGAME_S2C_PlayerLeave msg;
        msg.nPlayerID = rPlayer.GetPlayerID();
        _BroadCastMsg(msg);
        if (m_pRoomMaster == NULL)
        {
            map< int, Player * >::iterator itMain = m_PlayerMap.begin();
            m_pRoomMaster = itMain->second;
            if (m_pRoomMaster)
            {
                MSG_CARDGAME_S2C_RoomMaster msg;
                msg.nPlayerID = m_pRoomMaster->GetPlayerID();
                _BroadCastMsg(msg);
            }
        }
    }
    if (m_bGameStarted)
    {
        if (bCurrentPlayer)
        {
            Player * pCurPlayer = _CurrentPlayer();
            if (pCurPlayer)
            {
                MSG_CARDGAME_S2C_CurrentTurnPlayer msg;
                msg.nPlayerID = pCurPlayer->GetPlayerID();
                _BroadCastMsg(msg);
            }
        }
        if (m_PlayerMap.size() == 1)
        {
            if (m_pRoomMaster)
            {
                MSG_CARDGAME_S2C_Win msg;
                msg.nPlayerID = m_pRoomMaster->GetPlayerID();
                m_pRoomMaster->SendMsg(msg);
            }
            m_bGameStarted = false;
        }
    }
    return true;
}

int Room::StartGame(Player & rPlayer)
{
    if (!m_bIsValid)
    {
        return MSG_CARDGAME_S2C_StartGameFailed::Reason_RoomIsInvalid;
    }
    if (m_bGameStarted)
    {
        return MSG_CARDGAME_S2C_StartGameFailed::Reason_GameHasStarted;
    }
    if (&rPlayer != m_pRoomMaster)
    {
        return MSG_CARDGAME_S2C_StartGameFailed::Reason_NotRoomMaster;
    }
    if (m_PlayerMap.size() <=1)
    {
        return MSG_CARDGAME_S2C_StartGameFailed::Reason_PlayerIsNotEnough;
    }
    else
    {
        m_bGameStarted = true;
    }

    MSG_CARDGAME_S2C_StartGame msg;
    _BroadCastMsg(msg);

    // init owner card
    _ResetCardTypeIndex();
    int nCardTypePos = 0;
    map< int, Player * >::iterator it = m_PlayerMap.begin();
    for (; it!= m_PlayerMap.end(); ++it)
    {
        Player * pPlayer = it->second;
        if (pPlayer)
        {
            pPlayer->SetCardType(m_aCardType[nCardTypePos]);

            MSG_CARDGAME_S2C_SetCardType msg;
            msg.nCardType = m_aCardType[nCardTypePos];
            pPlayer->SendMsg(msg);

            nCardTypePos++;
        }
    }

    // init path
    for (unsigned int i=0; i<cMAX_PATH_LEN; i++)
    {
        m_Path[i].Reset();
    }
    m_Path[0].AddCardType(CardType_Red);
    m_Path[0].AddCardType(CardType_Blue);
    m_Path[0].AddCardType(CardType_Green);
    m_Path[0].AddCardType(CardType_Purple);
    m_Path[0].AddCardType(CardType_Yellow);
    for (unsigned int i=0; i<cMAX_USER_CARD_TYPE; i++)
    {
        m_aCardTypePath[i] = 0;
    }

    // init cardlist
    _ResetCardListIndex();
    m_nCurrentCardPos = 0;

    // get cards to each player
    it = m_PlayerMap.begin();
    for (; it!=m_PlayerMap.end(); ++it)
    {
        Player * pPlayer = it->second;
        if (pPlayer)
        {
            for (unsigned int i=0; i<cMAX_CARDS_PER_PLAYER; i++)
            {
                const Card & rCard = GetANewCard();
                pPlayer->InitCard(i, rCard);
            }
        }
    }

    // send the current turn player
    m_it = m_PlayerMap.begin();
    if (m_it->second)
    {
        MSG_CARDGAME_S2C_CurrentTurnPlayer msg;
        msg.nPlayerID = m_it->second->GetPlayerID();
        _BroadCastMsg(msg);
    }
    return MSG_CARDGAME_S2C_StartGameFailed::Reason_Success;
}

int Room::UseCard(Player & rPlayer, const Card & rCard, CardType eCardType)
{
    WriteLog(LEVEL_DEBUG, "Room::UseCard. PlayerID=%d, Card(%d, %d), CardType=%d.\n", rPlayer.GetPlayerID(), rCard.m_eCardType, rCard.m_eCardInstruction, eCardType);
    if (_CurrentPlayer() != &rPlayer)
    {
        return MSG_CARDGAME_S2C_UseCardFailed::Reason_IsNotYourTurn;
    }
    if (eCardType != rCard.m_eCardType && rCard.m_eCardType != CardType_MultiColour)
    {
        return MSG_CARDGAME_S2C_UseCardFailed::Reason_CardTypeIsError;
    }

    MSG_CARDGAME_S2C_UseCard useCardMsg;
    useCardMsg.nCardType = rCard.m_eCardType;
    useCardMsg.nCardInstruction = rCard.m_eCardInstruction;
    useCardMsg.nTargetCardType = eCardType;
    _BroadCastMsg(useCardMsg);

    int nPathID = m_aCardTypePath[eCardType];
    if (nPathID == 0)
    {
        if (rCard.m_eCardInstruction > 0)
        {
            int nNewPathID = rCard.m_eCardInstruction;
            m_Path[nNewPathID].AddCardType(eCardType);
            m_aCardTypePath[eCardType] = nNewPathID;
            m_Path[0].RemoveCardType(eCardType);

            MSG_CARDGAME_S2C_MoveCard msg;
            msg.nCardType = eCardType;
            msg.nSrcPathID = 0;
            msg.nDesPathID = nNewPathID;
            _BroadCastMsg(msg);
        }
    }
    else if (nPathID > 0)
    {
        int nNewPathID = nPathID + rCard.m_eCardInstruction;
        if (nNewPathID < 0)
        {
            nNewPathID = 0;
        }
        else if (nNewPathID > 9)
        {
            nNewPathID = 9;
        }
        CardCollection cardCollection = m_Path[nPathID].GetCardCollection(eCardType);
        m_Path[nPathID].MoveCardCollectionTo(eCardType, m_Path[nNewPathID]);
        stack< CardType > cardTypeStack = cardCollection.GetCardTypeStack();
        stack< CardType > cardTypeStackRev;
        while (!cardTypeStack.empty())
        {
            m_aCardTypePath[cardTypeStack.top()] = nNewPathID;
            cardTypeStackRev.push(cardTypeStack.top());
            cardTypeStack.pop();
        }
        while (!cardTypeStackRev.empty())
        {
            MSG_CARDGAME_S2C_MoveCard msg;
            msg.nCardType = cardTypeStackRev.top();
            msg.nSrcPathID = nPathID;
            msg.nDesPathID = nNewPathID;
            _BroadCastMsg(msg);

            cardTypeStackRev.pop();
        }
    }
    else
    {
        return MSG_CARDGAME_S2C_UseCardFailed::Reason_PathError;
    }
    nPathID = m_aCardTypePath[eCardType];
    if (nPathID == 9)
    {
        // win
        WriteLog(LEVEL_DEBUG, "Win. CardType: %d.\n", eCardType);
        bool bHaveWinner = false;
        map< int, Player * >::iterator it = m_PlayerMap.begin();
        for (; it!=m_PlayerMap.end(); ++it)
        {
            Player * pPlayer = it->second;
            if (pPlayer)
            {
                if (pPlayer->GetCardType() == eCardType)
                {
                    MSG_CARDGAME_S2C_Win msg;
                    msg.nPlayerID = pPlayer->GetPlayerID();
                    _BroadCastMsg(msg);
                    bHaveWinner = true;
                }
            }
        }
        if (!bHaveWinner)
        {
            MSG_CARDGAME_S2C_Win msg;
            msg.nPlayerID = -1;
            _BroadCastMsg(msg);
            for (; it!=m_PlayerMap.end(); ++it)
            {
                Player * pPlayer = it->second;
                if (pPlayer)
                {
                    UpdateGameResultDBQuery * pDBQuery = new UpdateGameResultDBQuery();
                    pDBQuery->m_nPlayerID = pPlayer->GetPlayerID();
                    pDBQuery->m_nScore = 1;
                    pDBQuery->m_GameResult = UpdateGameResultDBQuery::GameResult_Dogfall;
                    DBQueryFactory::Instance().AddTask(CGDBQTT_UpdateGameResult, pDBQuery);
                }
            }
        }
        else
        {
            for (; it!=m_PlayerMap.end(); ++it)
            {
                Player * pPlayer = it->second;
                if (pPlayer)
                {
                    UpdateGameResultDBQuery * pDBQuery = new UpdateGameResultDBQuery();
                    pDBQuery->m_nPlayerID = pPlayer->GetPlayerID();
                    pDBQuery->m_nScore = 0;
                    pDBQuery->m_GameResult = UpdateGameResultDBQuery::GameResult_Lose;
                    if (pPlayer->GetCardType() == eCardType)
                    {
                        pDBQuery->m_nScore = 10;
                        pDBQuery->m_GameResult = UpdateGameResultDBQuery::GameResult_Win;
                    }
                    DBQueryFactory::Instance().AddTask(CGDBQTT_UpdateGameResult, pDBQuery);
                }
            }
        }
        m_bGameStarted = false;
    }
    else
    {
        _TurnToNextPlayer();
        Player * pCurPlayer = _CurrentPlayer();
        if (pCurPlayer)
        {
            MSG_CARDGAME_S2C_CurrentTurnPlayer msg;
            msg.nPlayerID = pCurPlayer->GetPlayerID();
            _BroadCastMsg(msg);
        }
    }
    return MSG_CARDGAME_S2C_UseCardFailed::Reason_Success;
}

const Card & Room::GetANewCard()
{
    const Card & rCard = g_CardList.m_aCard[m_aCardListIndex[m_nCurrentCardPos]];
    m_nCurrentCardPos++;
    if (m_nCurrentCardPos >= cMAX_CARD_LIST_NUM)
    {
        _ResetCardListIndex();
        m_nCurrentCardPos = 0;
    }
    return rCard;
}

/*
   void Room::PrintRoom()
   {
   for (int i=0; i<10; i++)
   {
   WriteLog(LEVEL_DEBUG, "Path(%d): ", m_Path[i].GetPathID());
   stack< CardType > cardTypeStack = m_Path[i].GetCardCollection().GetCardTypeStack();
   while (!cardTypeStack.empty())
   {
   WriteLog(LEVEL_DEBUG, "%d  ", cardTypeStack.top());
   cardTypeStack.pop();
   }
   WriteLog(LEVEL_DEBUG, ".\n");
   }
   }
 */

int Room::GetRoomID()
{
    return m_nRoomID;
}

int Room::GetPlayerNumber()
{
    return m_PlayerMap.size();
}

Player * Room::_CurrentPlayer()
{
    if (m_it != m_PlayerMap.end())
    {
        return m_it->second;
    }
    else
    {
        return NULL;
    }
}

void Room::_TurnToNextPlayer()
{
    ++m_it;
    if (m_it == m_PlayerMap.end())
    {
        m_it = m_PlayerMap.begin();
    }
}

void Room::_ResetCardTypeIndex()
{
    for (unsigned int i=0; i<cMAX_USER_CARD_TYPE; i++)
    {
        int nRand = rand()%cMAX_USER_CARD_TYPE;
        CardType nTmp = m_aCardType[i];
        m_aCardType[i] = m_aCardType[nRand];
        m_aCardType[nRand] = nTmp;
    }
}

void Room::_ResetCardListIndex()
{
    for (unsigned int i=0; i<cMAX_CARD_LIST_NUM; i++)
    {
        int nRand = rand()%cMAX_CARD_LIST_NUM;
        int nTmp = m_aCardListIndex[i];
        m_aCardListIndex[i] = m_aCardListIndex[nRand];
        m_aCardListIndex[nRand] = nTmp;
    }
}
