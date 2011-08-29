#include "roommgr.h"
#include "serverfunc.h"
#include "cardgamemsg.h"
#include "cardgame.h"

RoomMgr::RoomMgr()
{
}

RoomMgr::~RoomMgr()
{
}

Room * RoomMgr::CreateRoom()
{
    Room * pRoom = new Room();
    return pRoom;
}
Room * RoomMgr::GetRoom(int nRoomID)
{
    Room * pRoom = NULL;
    return pRoom;
}

void RoomMgr::SendRoomInfo(CardSlot & rSlot)
{
    MSG_CARDGAME_S2C_RoomInfo msg;
    msg.nRoomNumber = m_RoomList.size();
    int nIndex = rSlot.m_nPage * cMAX_PAGE_ROOM_NUM;
    while (nIndex >= 0)
    {
        if (nIndex >= (int)(m_RoomList.size()))
        {
            nIndex -= cMAX_PAGE_ROOM_NUM;
            if (rSlot.m_nPage > 0)
            {
                rSlot.m_nPage--;
            }
            continue;
        }
        else
        {
            list< Room * >::iterator it = m_RoomList.begin();
            for (int i=0; i<nIndex; i++)
            {
                ++it;
            }
            for (unsigned int i=0; it!=m_RoomList.end() && i<cMAX_PAGE_ROOM_NUM; i++, ++it)
            {
                msg.aRoomInfo[i].nRoomID = (*it)->GetRoomID();
                msg.aRoomInfo[i].nRoomIndex = nIndex + i;
                msg.aRoomInfo[i].nPlayerCount = (*it)->GetPlayerNumber();
            }
            break;
        }
    }
    msg.nCurrentPage = rSlot.m_nPage;
    ::SendMsg(msg, rSlot.m_nSlotIndex);
}

const list< Room * > & RoomMgr::GetRoomList()
{
    return m_RoomList;
}
