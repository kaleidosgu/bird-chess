#include "path.h"

Path::Path()
{
    m_nPathID = -1;
}

Path::~Path()
{
}

void Path::Reset()
{
    m_CardCollection.Reset();
}

bool Path::AddCardType(CardType eCardType)
{
    return m_CardCollection.AddCardType(eCardType);
}

bool Path::RemoveCardType(CardType eCardType)
{
    if (m_nPathID == 0)
    {
        return m_CardCollection.RemoveCardType(eCardType);
    }
    else
    {
        return false;
    }
}

CardCollection Path::GetCardCollection(CardType eCardType)
{
    return m_CardCollection.GetSubCollection(eCardType);
}

CardCollection & Path::GetCardCollection()
{
    return m_CardCollection;
}

bool Path::MoveCardCollectionTo(CardType eCardType, Path & rDesPath)
{
    return m_CardCollection.MoveCardTo(eCardType, rDesPath.m_CardCollection);
}

void Path::SetPathID(int nPathID)
{
    m_nPathID = nPathID;
}

int Path::GetPathID()
{
    return m_nPathID;
}
