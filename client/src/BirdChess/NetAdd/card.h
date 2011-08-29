#ifndef __cardgame_card_h_
#define __cardgame_card_h_

#include <stack>

using namespace std;

enum CardType
{
    CardType_Red,
    CardType_Blue,
    CardType_Green,
    CardType_Purple,
    CardType_Yellow,
    CardType_MultiColour,
    CardType_Unknown,
};

enum CardInstruction
{
    Back_7 = -7,
    Back_6 = -6,
    Back_5 = -5,
    Back_4 = -4,
    Back_3 = -3,
    Back_2 = -2,
    Back_1 = -1,
    Forward_1 = 1,
    Forward_2 = 2,
    Forward_3 = 3,
    Forward_4 = 4,
    Forward_5 = 5,
    Forward_6 = 6,
    Forward_7 = 7,
};

class Card
{
public:
    Card()
    {
        m_eCardType = CardType_MultiColour;
        m_eCardInstruction = Forward_1;
    }
    CardType m_eCardType;
    CardInstruction m_eCardInstruction;
};

class CardCollection
{
public:
    CardCollection();
    virtual ~CardCollection();
    //bool IsEmpty();
    void Reset();
    bool AddCardType(CardType eCardType);
    bool RemoveCardType(CardType eCardType);
    CardCollection GetSubCollection(CardType eCardType);
    bool MoveCardTo(CardType eCardType, CardCollection & rCardCollection);

    stack< CardType > GetCardTypeStack();
private:
    stack < CardType > m_CardTypeStack;
};

#endif // __cardgame_card_h_
