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
    for (unsigned int i=0; i<=cMAX_PATH_LEN; i++)
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
        WriteLog(LEVEL_DEBUG, "Room(%d)::PlayerEnter. A player(%d) Enter the room.\n", m_nRoomID, rPlayer.GetPlayerID());
        // Broadcast
        MSG_CARDGAME_S2C_PlayerEnter msg;
        msg.nPlayerID = rPlayer.GetPlayerID();
        strncpy(msg.szPlayerName, rPlayer.GetPlayerName().c_str(), cMAX_USERNAME_LEN - 1);
        msg.nScore = rPlayer.GetScore();
        msg.nWin = rPlayer.GetWin();
        msg.nDogfall = rPlayer.GetDogfall();
        msg.nLose = rPlayer.GetLose();
        BroadCastMsg(msg);

        // first player
        if (m_pRoomMaster == NULL)
        {
            m_pRoomMaster = &rPlayer;
            WriteLog(LEVEL_DEBUG, "Room(%d)::PlayerEnter. There is no room master, so set the player(%d) to be the room master.\n", m_nRoomID, rPlayer.GetPlayerID());
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

void Room::BroadCastMsg(MSG_BASE & rMsg)
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
    WriteLog(LEVEL_DEBUG, "Room(%d)::PlayerLeave.\n", m_nRoomID);
    // confirm the player is in the room
    map< int, Player * >::iterator it = m_PlayerMap.find(rPlayer.GetPlayerID());
    if (it == m_PlayerMap.end())
    {
        WriteLog(LEVEL_DEBUG, "Room(%d)::PlayerLeave. Can't find the player(%d).\n", m_nRoomID, rPlayer.GetPlayerID());
        return false;
    }

    if (&rPlayer == m_pRoomMaster)
    {
        WriteLog(LEVEL_DEBUG, "Room(%d)::PlayerLeave. The player(%d) is the room master. So reset the room master.\n", m_nRoomID, rPlayer.GetPlayerID());
        m_pRoomMaster = NULL;
    }
    bool bCurrentPlayer = false;
    if (m_bGameStarted && it== m_it)
    {
        WriteLog(LEVEL_DEBUG, "Room(%d)::PlayerLeave. The game is started, and the player(%d) is current turn player. So turn to next player.\n", m_nRoomID, rPlayer.GetPlayerID());
        bCurrentPlayer = true;
        _TurnToNextPlayer();
    }
    m_PlayerMap.erase(it);
    if (m_PlayerMap.empty())
    {
        // No player, and set room invalid
        WriteLog(LEVEL_DEBUG, "Room(%d)::PlayerLeave. No player in the room now, so set the room invalid.\n", m_nRoomID);
        m_bIsValid = false;
    }
    else
    {
        // Broadcast
        MSG_CARDGAME_S2C_PlayerLeave msg;
        msg.nPlayerID = rPlayer.GetPlayerID();
        BroadCastMsg(msg);
        if (m_pRoomMaster == NULL)
        {
            map< int, Player * >::iterator itMain = m_PlayerMap.begin();
            m_pRoomMaster = itMain->second;
            if (m_pRoomMaster)
            {
                WriteLog(LEVEL_DEBUG, "Room(%d)::PlayerLeave. Reset the room master to player(%d).\n", m_nRoomID, m_pRoomMaster->GetPlayerID());
                MSG_CARDGAME_S2C_RoomMaster msg;
                msg.nPlayerID = m_pRoomMaster->GetPlayerID();
                BroadCastMsg(msg);
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
                WriteLog(LEVEL_DEBUG, "Room(%d)::PlayerLeave. Reset the current turn player(%d).\n", m_nRoomID, pCurPlayer->GetPlayerID());
                MSG_CARDGAME_S2C_CurrentTurnPlayer msg;
                msg.nPlayerID = pCurPlayer->GetPlayerID();
                BroadCastMsg(msg);
            }
        }
        if (m_PlayerMap.size() == 1)
        {
            if (m_pRoomMaster)
            {
                MSG_CARDGAME_S2C_Win msg;
                msg.nPlayerID = m_pRoomMaster->GetPlayerID();
                m_pRoomMaster->SendMsg(msg);
                WriteLog(LEVEL_DEBUG, "Room(%d)::PlayerLeave. The Game is started, and there is only one player in the room. So the player(%d) win.\n", m_nRoomID, m_pRoomMaster->GetPlayerID());
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
        WriteLog(LEVEL_DEBUG, "Room(%d)::StartGame. Start game failed, because the room is invalid.\n", m_nRoomID);
        return MSG_CARDGAME_S2C_StartGameFailed::Reason_RoomIsInvalid;
    }
    if (m_bGameStarted)
    {
        WriteLog(LEVEL_DEBUG, "Room(%d)::StartGame. Start game failed, because the game has started.\n", m_nRoomID);
        return MSG_CARDGAME_S2C_StartGameFailed::Reason_GameHasStarted;
    }
    if (&rPlayer != m_pRoomMaster)
    {
        WriteLog(LEVEL_DEBUG, "Room(%d)::StartGame. Start game failed, because the player(%d) is not the room master.\n", m_nRoomID, rPlayer.GetPlayerID());
        return MSG_CARDGAME_S2C_StartGameFailed::Reason_NotRoomMaster;
    }
    if (m_PlayerMap.size() <=1)
    {
        WriteLog(LEVEL_DEBUG, "Room(%d)::StartGame. Start game failed, because player is not enough.\n", m_nRoomID);
        return MSG_CARDGAME_S2C_StartGameFailed::Reason_PlayerIsNotEnough;
    }
    else
    {
        m_bGameStarted = true;
    }

    MSG_CARDGAME_S2C_StartGame msg;
    BroadCastMsg(msg);
    WriteLog(LEVEL_DEBUG, "Room(%d)::StartGame. start the game.\n", m_nRoomID);

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

            WriteLog(LEVEL_DEBUG, "Room(%d)::StartGame. Send the CardType(%d) to the player(%d).\n", m_nRoomID, m_aCardType[nCardTypePos], pPlayer->GetPlayerID());

            MSG_CARDGAME_S2C_SetCardType msg;
            msg.nCardType = m_aCardType[nCardTypePos];
            pPlayer->SendMsg(msg);

            nCardTypePos++;
        }
    }

    // init path
    WriteLog(LEVEL_DEBUG, "Room(%d)::StartGame. Init the path.\n", m_nRoomID);
    for (unsigned int i=0; i<=cMAX_PATH_LEN; i++)
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
    WriteLog(LEVEL_DEBUG, "Room(%d)::StartGame. Init the card list.\n", m_nRoomID);
    _ResetCardListIndex();
    m_nCurrentCardPos = 0;

    // get cards to each player
    WriteLog(LEVEL_DEBUG, "Room(%d)::StartGame. Init the players' cards.\n", m_nRoomID);
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
        WriteLog(LEVEL_DEBUG, "Room(%d)::StartGame. Set the current turn player(%d).\n", m_nRoomID, m_it->second->GetPlayerID());
        MSG_CARDGAME_S2C_CurrentTurnPlayer msg;
        msg.nPlayerID = m_it->second->GetPlayerID();
        BroadCastMsg(msg);
    }
    return MSG_CARDGAME_S2C_StartGameFailed::Reason_Success;
}

int Room::UseCard(Player & rPlayer, const Card & rCard, CardType eCardType)
{
    WriteLog(LEVEL_DEBUG, "Room(%d)::UseCard. PlayerID=%d, Card(%d, %d), CardType=%d.\n", m_nRoomID, rPlayer.GetPlayerID(), rCard.m_eCardType, rCard.m_eCardInstruction, eCardType);
    if (!m_bGameStarted)
    {
        WriteLog(LEVEL_DEBUG, "Room(%d)::UseCard. Use card failed as the game is not started.\n", m_nRoomID);
        return MSG_CARDGAME_S2C_UseCardFailed::Reason_GameNotStarted;
    }
    if (_CurrentPlayer() != &rPlayer)
    {
        WriteLog(LEVEL_DEBUG, "Room(%d)::UseCard. Use card failed as the player(%d) is not the current turn player.\n", m_nRoomID, rPlayer.GetPlayerID());
        return MSG_CARDGAME_S2C_UseCardFailed::Reason_IsNotYourTurn;
    }
    if (eCardType != rCard.m_eCardType && rCard.m_eCardType != CardType_MultiColour)
    {
        WriteLog(LEVEL_DEBUG, "Room(%d)::UseCard. Use card failed as the cardtype(%d) is error.\n", m_nRoomID, eCardType);
        return MSG_CARDGAME_S2C_UseCardFailed::Reason_CardTypeIsError;
    }

    MSG_CARDGAME_S2C_UseCard useCardMsg;
    useCardMsg.nCardType = rCard.m_eCardType;
    useCardMsg.nCardInstruction = rCard.m_eCardInstruction;
    useCardMsg.nTargetCardType = eCardType;
    BroadCastMsg(useCardMsg);

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
            BroadCastMsg(msg);
            WriteLog(LEVEL_DEBUG, "Room(%d)::UseCard. Move Card(%d) from (%d) to (%d).\n", m_nRoomID, eCardType, 0, nNewPathID);
        }
    }
    else if (nPathID > 0)
    {
        int nNewPathID = nPathID + rCard.m_eCardInstruction;
        if (nNewPathID < 1)
        {
            nNewPathID = 1;
        }
        else if (nNewPathID > cMAX_PATH_LEN)
        {
            nNewPathID = cMAX_PATH_LEN;
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
            BroadCastMsg(msg);
            WriteLog(LEVEL_DEBUG, "Room(%d)::UseCard. Move Card(%d) from (%d) to (%d).\n", m_nRoomID, eCardType, nPathID, nNewPathID);

            cardTypeStackRev.pop();
        }
    }
    else
    {
        WriteLog(LEVEL_DEBUG, "Room(%d)::UseCard. Use card failed as the path(%d) is error.\n", m_nRoomID, nPathID);
        return MSG_CARDGAME_S2C_UseCardFailed::Reason_PathError;
    }
    nPathID = m_aCardTypePath[eCardType];
    WriteLog(LEVEL_DEBUG, "Room(%d)::UseCard. Use Card Result. CardType=%d, Path=%d.\n", m_nRoomID, eCardType, nPathID);
    if (nPathID == cMAX_PATH_LEN)
    {
        // win
        //WriteLog(LEVEL_DEBUG, "Room(%d)::UseCard. CardType(%d) win.\n", m_nRoomID, eCardType);
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
                    BroadCastMsg(msg);
                    WriteLog(LEVEL_DEBUG, "Room(%d)::UseCard. CardType(%d) win. Player(%d) win.\n", m_nRoomID, eCardType, pPlayer->GetPlayerID());
                    bHaveWinner = true;
                }
            }
        }
        it = m_PlayerMap.begin();
        if (!bHaveWinner)
        {
            MSG_CARDGAME_S2C_Win msg;
            msg.nPlayerID = -1;
            BroadCastMsg(msg);
            WriteLog(LEVEL_DEBUG, "Room(%d)::UseCard. CardType(%d) win. But no one win.\n", m_nRoomID, eCardType);
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
            BroadCastMsg(msg);
            WriteLog(LEVEL_DEBUG, "Room(%d)::UseCard. Turn to next player(%d).\n", m_nRoomID, pCurPlayer->GetPlayerID());
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
