#include "card.h"

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
