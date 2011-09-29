#ifndef __cardgame_room_h
#define __cardgame_room_h

#include "path.h"
#include "player.h"
#include <map>
#include "cardgamemsg.h"

using namespace std;

class Room
{
public:
    Room();
    virtual ~Room();

    int PlayerEnter(Player & rPlayer);
    bool PlayerLeave(Player & rPlayer);
    void SetInvalid()
    {
        m_bIsValid = false;
    }
    bool IsValid()
    {
        return m_bIsValid;
    }
    int StartGame(Player & rPlayer);
    int UseCard(Player & rPlayer, const Card & rCard, CardType eCardType);
    const Card & GetANewCard();

    //void PrintRoom();

    int GetRoomID();
    int GetPlayerNumber();
    void BroadCastMsg(MSG_BASE & rMsg);

private:
    Player * _CurrentPlayer();
    void _TurnToNextPlayer();
    void _SendAllPlayerInfoToPlayer(Player & rPlayer);
    void _ResetCardTypeIndex();
    void _ResetCardListIndex();


    int m_nRoomID;
    bool m_bGameStarted;
    bool m_bIsValid;

    Player * m_pRoomMaster;

    map< int, Player * > m_PlayerMap;
    map< int, Player * >::iterator m_it;

    Path m_Path[cMAX_PATH_LEN + 1];

    // card type
    CardType m_aCardType[cMAX_USER_CARD_TYPE];
    int m_aCardTypePath[cMAX_USER_CARD_TYPE];

    // card list
    int m_aCardListIndex[cMAX_CARD_LIST_NUM];
    unsigned int m_nCurrentCardPos;
};

#endif // __cardgame_room_h
