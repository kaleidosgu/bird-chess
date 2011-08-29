#ifndef __cardgame_roommgr_h_
#define __cardgame_roommgr_h_

#include <list>
#include "room.h"

using namespace std;

class CardSlot;
class RoomMgr
{
public:
    RoomMgr();
    virtual ~RoomMgr();

    Room * CreateRoom();

    Room * GetRoom(int nRoomID);

    void SendRoomInfo(CardSlot & rSlot);

    const list< Room * > & GetRoomList();
private:
    list< Room * > m_RoomList;
};

#endif // __cardgame_roommgr_h_
