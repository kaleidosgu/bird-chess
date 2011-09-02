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

    void HeartBeat(unsigned int nIntervalMS);
private:
    map< int, Room * > m_IDMapRoom;
};

#endif // __cardgame_roommgr_h_
