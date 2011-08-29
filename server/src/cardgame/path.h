#ifndef __cardgame_path_h_
#define __cardgame_path_h_

#include "card.h"

class Path
{
public:
    Path();
    virtual ~Path();
    void Reset();
    bool AddCardType(CardType eCardType);
    bool RemoveCardType(CardType eCardType);

    CardCollection GetCardCollection(CardType eCardType);
    bool MoveCardCollectionTo(CardType eCardType, Path & rDesPath);

    void SetPathID(int nPathID);
    int GetPathID();
    //bool HasAnyCard();
    //int GetTheCardIndex(CardType);
    CardCollection & GetCardCollection();
private:
    int m_nPathID;
    CardCollection m_CardCollection;
};

#endif // __cardgame_path_h_
