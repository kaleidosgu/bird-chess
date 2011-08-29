#ifndef __cardgame_player_h_
#define __cardgame_player_h_

#include "card.h"
#include <string>
#include "cardgamemsg.h"

using namespace std;

class Room;
class CardSlot;

class Player
{
public:
    Player(CardSlot & rSlot, int nPlayerID, string strPlayerName, int nScore, int nWin, int nDogfall, int nLose);
    virtual ~Player();
    int GetPlayerID();
    const string & GetPlayerName();
    int GetScore();
    int GetWin();
    int GetDogfall();
    int GetLose();
    int EnterRoom(Room * pRoom);
    bool LeaveRoom();
    int StartGame();
    int UseCard(unsigned int nCardPos, CardType eCardType);
    //bool UseCard1(CardType eCardType);
    //bool UseCard2(CardType eCardType);
    
    void SetCardType(CardType eCardType);
    CardType GetCardType();
    bool InitCard(unsigned int nCardPos, const Card & rCard);
    //void InitCards(const Card & rCard1, const Card & rCard2, const Card & rCard3);

    void ProcessMsg(MSG_BASE & rMsg);
    bool SendMsg(MSG_BASE & rMsg);

protected:
    /*
    Card * _GetCard1();
    Card * _GetCard2();
    bool _ReleaseCard1();
    bool _ReleaseCard2();
    bool _GetANewCard(Card * pCard);
    */

    CardSlot & m_rSlot;
    int m_nPlayerID;
    string m_strPlayerName;
    int m_nScore;
    int m_nWin;
    int m_nDogfall;
    int m_nLose;
    CardType m_eCardType;
    int m_aCards[cMAX_CARDS_PER_PLAYER];
    Room * m_pRoom;
};

#endif // __cardgame_player_h_
