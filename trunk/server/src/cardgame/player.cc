#include "player.h"
#include "room.h"
#include "serverfunc.h"
#include "cardgame.h"

extern const CardList g_CardList;

Player::Player(CardSlot & rSlot, int nPlayerID, string strPlayerName, int nScore, int nWin, int nDogfall, int nLose) :
    m_rSlot(rSlot)
{
    m_nPlayerID = nPlayerID;
    m_strPlayerName = strPlayerName;
    m_nScore = nScore;
    m_nWin = nWin;
    m_nDogfall = nDogfall;
    m_nLose = nLose;

    m_eCardType = CardType_Unknown;
    for (unsigned int i=0; i<cMAX_CARDS_PER_PLAYER; i++)
    {
        m_aCards[i] = -1;
    }

    m_pRoom = NULL;
}

Player::~Player()
{
}

int Player::GetPlayerID()
{
    return m_nPlayerID;
}

const string & Player::GetPlayerName()
{
    return m_strPlayerName;
}

int Player::GetScore()
{
    return m_nScore;
}

int Player::GetWin()
{
    return m_nWin;
}

int Player::GetDogfall()
{
    return m_nDogfall;
}

int Player::GetLose()
{
    return m_nLose;
}

int Player::EnterRoom(Room * pRoom)
{
    if (NULL == pRoom)
    {
        return MSG_CARDGAME_S2C_EnterRoomResult::Result_PlayerRoomError;
    }
    else
    {
        m_pRoom = pRoom;
        return m_pRoom->PlayerEnter(*this);
    }
}

bool Player::LeaveRoom()
{
    if (NULL == m_pRoom)
    {
        return false;
    }
    else
    {
        WriteLog(LEVEL_DEBUG, "Player::LeaveRoom. PlayerID=%d, RoomID=%d.\n", m_nPlayerID, m_pRoom->GetRoomID());
        if (m_pRoom->PlayerLeave(*this))
        {
            m_pRoom = NULL;
            return true;
        }
        else
        {
            return false;
        }
    }
}

int Player::StartGame()
{
    if (m_pRoom == NULL)
    {
        return MSG_CARDGAME_S2C_StartGameFailed::Reason_PlayerRoomError;
    }
    else
    {
        return m_pRoom->StartGame(*this);
    }
}

int Player::UseCard(unsigned int nCardPos, CardType eCardType)
{
    if (nCardPos < 0 && nCardPos >= cMAX_CARDS_PER_PLAYER)
    {
        return MSG_CARDGAME_S2C_UseCardFailed::Reason_CardPosIsError;
    }
    if (m_aCards[nCardPos] < 0 && m_aCards[nCardPos] >= (int)cMAX_CARD_LIST_NUM)
    {
        return MSG_CARDGAME_S2C_UseCardFailed::Reason_CardIndexError;
    }
    if (eCardType < CardType_Red || eCardType >= CardType_MultiColour)
    {
        return MSG_CARDGAME_S2C_UseCardFailed::Reason_CardTypeIsError;
    }
    if (m_pRoom == NULL)
    {
        return MSG_CARDGAME_S2C_UseCardFailed::Reason_PlayerRoomError;
    }
    const Card & rCard = g_CardList.m_aCard[m_aCards[nCardPos]];
    if (rCard.m_eCardType != CardType_MultiColour && rCard.m_eCardType != eCardType)
    {
        return MSG_CARDGAME_S2C_UseCardFailed::Reason_CardIsError;
    }
    int nRes = m_pRoom->UseCard(*this, rCard, eCardType);
    if (nRes == MSG_CARDGAME_S2C_UseCardFailed::Reason_Success)
    {
        const Card & rNewCard = m_pRoom->GetANewCard();

        m_aCards[nCardPos] = rNewCard.m_nCardID;
        MSG_CARDGAME_S2C_AddNewCard msg;
        msg.nCardPos = nCardPos;
        msg.nCardID = rNewCard.m_nCardID;
        msg.nCardType = rNewCard.m_eCardType;
        msg.nCardInstruction = rNewCard.m_eCardInstruction;
        SendMsg(msg);
    }

    return nRes;
}

void Player::SetCardType(CardType eCardType)
{
    m_eCardType = eCardType;
}

CardType Player::GetCardType()
{
    return m_eCardType;
}

bool Player::InitCard(unsigned int nCardPos, const Card & rCard)
{
    if (nCardPos < 0 && nCardPos >= cMAX_CARDS_PER_PLAYER)
    {
        return false;
    }
    if (m_aCards[nCardPos] != -1)
    {
        return false;
    }
    else
    {
        m_aCards[nCardPos] = rCard.m_nCardID;
        MSG_CARDGAME_S2C_AddNewCard msg;
        msg.nCardPos = nCardPos;
        msg.nCardID = rCard.m_nCardID;
        msg.nCardType = rCard.m_eCardType;
        msg.nCardInstruction = rCard.m_eCardInstruction;
        SendMsg(msg);
        return true;
    }
}

//void Player::InitCards(const Card & rCard1, const Card & rCard2, const Card & rCard3)
//{
//}

/*
   bool Player::UseCard1(CardType eCardType)
   {
   if (m_pCard1 == NULL)
   {
   return false;
   }
   else
   {
   if (m_pRoom)
   {
   bool bRes = true;
   if (m_pCard1->m_eCardType == CardType_MultiColour)
   {
   bRes = m_pRoom->UseCard(*this, *m_pCard1, eCardType);
   }
   else
   {
   bRes = m_pRoom->UseCard(*this, *m_pCard1, m_pCard1->m_eCardType);
   }
   if (bRes)
   {
   _ReleaseCard1();
   _GetANewCard(m_pRoom->GetANewCard());
   return true;
   }
   else
   {
   return false;
   }
   }
   else
   {
   return false;
   }
   }
   }

   bool Player::UseCard2(CardType eCardType)
   {
   if (m_pCard2 == NULL)
   {
   return false;
   }
   else
   {
   if (m_pRoom)
   {
   bool bRes = true;
   if (m_pCard2->m_eCardType == CardType_MultiColour)
   {
   bRes = m_pRoom->UseCard(*this, *m_pCard2, eCardType);
   }
   else
   {
   bRes = m_pRoom->UseCard(*this, *m_pCard2, m_pCard2->m_eCardType);
   }
   if (bRes)
   {
   _ReleaseCard2();
   _GetANewCard(m_pRoom->GetANewCard());
   return true;
   }
   else
   {
   return false;
   }
   }
   else
   {
   return false;
}
}
}

bool Player::_GetANewCard(Card * pCard)
{
    if (m_pCard1 == NULL)
    {
        m_pCard1 = pCard;
        MSG_CARDGAME_S2C_AddNewCard msg;
        msg.nCardID = 1;
        msg.nCardType = pCard->m_eCardType;
        msg.nCardInstruction = pCard->m_eCardInstruction;
        SendMsg(msg);
        return true;
    }
    else if (m_pCard2 == NULL)
    {
        m_pCard2 = pCard;
        MSG_CARDGAME_S2C_AddNewCard msg;
        msg.nCardID = 2;
        msg.nCardType = pCard->m_eCardType;
        msg.nCardInstruction = pCard->m_eCardInstruction;
        SendMsg(msg);
        return true;
    }
    else
    {
        return false;
    }
}

Card * Player::_GetCard1()
{
    return m_pCard1;
}

Card * Player::_GetCard2()
{
    return m_pCard2;
}

bool Player::_ReleaseCard1()
{
    if (m_pCard1 == NULL)
    {
        return false;
    }
    else
    {
        delete m_pCard1;
        m_pCard1 = NULL;
        return true;
    }
}

bool Player::_ReleaseCard2()
{
    if (m_pCard2 == NULL)
    {
        return false;
    }
    else
    {
        delete m_pCard2;
        m_pCard2 = NULL;
        return true;
    }
}
*/

bool Player::SendMsg(MSG_BASE & rMsg)
{
    return ::SendMsg(rMsg, m_rSlot.m_nSlotIndex);
}

void Player::ProcessMsg(MSG_BASE & rMsg)
{
    switch (rMsg.nMsg)
    {
        case MSGID_CARDGAME_C2S_StartGame :
            {
                int nResult = StartGame();
                if (nResult != MSG_CARDGAME_S2C_StartGameFailed::Reason_Success)
                {
                    MSG_CARDGAME_S2C_StartGameFailed msg;
                    msg.nReason = nResult;
                    SendMsg(msg);
                }
            }
            break;
        case MSGID_CARDGAME_C2S_UseCard :
            {
                MSG_CARDGAME_C2S_UseCard & rUseCardMsg = (MSG_CARDGAME_C2S_UseCard &)rMsg;

                int nResult = UseCard(rUseCardMsg.nCardPos, (CardType)(rUseCardMsg.nCardType));
                if (nResult != MSG_CARDGAME_S2C_UseCardFailed::Reason_Success)
                {
                    MSG_CARDGAME_S2C_UseCardFailed msg;
                    msg.nReason = nResult;
                    SendMsg(msg);
                }
            }
            break;
        default :
            break;
    }
}
