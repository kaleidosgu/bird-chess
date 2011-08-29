#include "card.h"

const CardList g_CardList;

static int s_nCardInfo[cMAX_CARD_LIST_NUM][2] =
{
    {CardType_Red, Back_2},
    {CardType_Red, Back_2},
    {CardType_Red, Back_1},
    {CardType_Red, Back_1},
    {CardType_Red, Forward_1},
    {CardType_Red, Forward_1},
    {CardType_Red, Forward_2},
    {CardType_Red, Forward_2},

    {CardType_Blue, Back_2},
    {CardType_Blue, Back_2},
    {CardType_Blue, Back_1},
    {CardType_Blue, Back_1},
    {CardType_Blue, Forward_1},
    {CardType_Blue, Forward_1},
    {CardType_Blue, Forward_2},
    {CardType_Blue, Forward_2},

    {CardType_Green, Back_2},
    {CardType_Green, Back_2},
    {CardType_Green, Back_1},
    {CardType_Green, Back_1},
    {CardType_Green, Forward_1},
    {CardType_Green, Forward_1},
    {CardType_Green, Forward_2},
    {CardType_Green, Forward_2},

    {CardType_Purple, Back_2},
    {CardType_Purple, Back_2},
    {CardType_Purple, Back_1},
    {CardType_Purple, Back_1},
    {CardType_Purple, Forward_1},
    {CardType_Purple, Forward_1},
    {CardType_Purple, Forward_2},
    {CardType_Purple, Forward_2},

    {CardType_Yellow, Back_2},
    {CardType_Yellow, Back_2},
    {CardType_Yellow, Back_1},
    {CardType_Yellow, Back_1},
    {CardType_Yellow, Forward_1},
    {CardType_Yellow, Forward_1},
    {CardType_Yellow, Forward_2},
    {CardType_Yellow, Forward_2},

    {CardType_MultiColour, Back_2},
    {CardType_MultiColour, Back_2},
    {CardType_MultiColour, Back_1},
    {CardType_MultiColour, Back_1},
    {CardType_MultiColour, Forward_1},
    {CardType_MultiColour, Forward_1},
    {CardType_MultiColour, Forward_2},
    {CardType_MultiColour, Forward_2}
};

CardList::CardList()
{
    for (unsigned int i=0; i<cMAX_CARD_LIST_NUM; i++)
    {
        m_aCard[i].m_eCardType = (CardType)(s_nCardInfo[i][0]);
        m_aCard[i].m_eCardInstruction = (CardInstruction)(s_nCardInfo[i][1]);
        m_aCard[i].m_nCardID = i;
    }
}

CardList::~CardList()
{
}

CardCollection::CardCollection()
{
}

CardCollection::~CardCollection()
{
}

void CardCollection::Reset()
{
    while (!m_CardTypeStack.empty())
    {
        m_CardTypeStack.pop();
    }
}

bool CardCollection::AddCardType(CardType eCardType)
{
    m_CardTypeStack.push(eCardType);
    return true;
}

bool CardCollection::RemoveCardType(CardType eCardType)
{
    bool bRes = false;
    stack< CardType > tStack;
    while (!m_CardTypeStack.empty())
    {
        if (eCardType == m_CardTypeStack.top())
        {
            bRes = true;
            m_CardTypeStack.pop();
            break;
        }
        else
        {
            tStack.push(m_CardTypeStack.top());
            m_CardTypeStack.pop();
        }
    }

    while (!tStack.empty())
    {
        m_CardTypeStack.push(tStack.top());
        tStack.pop();
    }
    return bRes;
}

CardCollection CardCollection::GetSubCollection(CardType eCardType)
{
    bool bRes = false;
    stack< CardType > tStack;
    while (!m_CardTypeStack.empty())
    {
        if (eCardType == m_CardTypeStack.top())
        {
            bRes = true;
            tStack.push(m_CardTypeStack.top());
            m_CardTypeStack.pop();
            break;
        }
        else
        {
            tStack.push(m_CardTypeStack.top());
            m_CardTypeStack.pop();
        }
    }

    CardCollection cardCollection;
    while (!tStack.empty())
    {
        if (bRes)
        {
            cardCollection.m_CardTypeStack.push(tStack.top());
        }
        m_CardTypeStack.push(tStack.top());
        tStack.pop();
    }
    return cardCollection;
}

bool CardCollection::MoveCardTo(CardType eCardType, CardCollection & rCardCollection)
{
    bool bRes = false;
    stack< CardType > tStack;
    while (!m_CardTypeStack.empty())
    {
        if (eCardType == m_CardTypeStack.top())
        {
            bRes = true;
            tStack.push(m_CardTypeStack.top());
            m_CardTypeStack.pop();
            break;
        }
        else
        {
            tStack.push(m_CardTypeStack.top());
            m_CardTypeStack.pop();
        }
    }

    while (!tStack.empty())
    {
        if (bRes)
        {
            rCardCollection.m_CardTypeStack.push(tStack.top());
        }
        else
        {
            m_CardTypeStack.push(tStack.top());
        }
        tStack.pop();
    }
    return bRes;
}

stack< CardType > CardCollection::GetCardTypeStack()
{
    return m_CardTypeStack;
}
