#include "roommgr.h"
#include "serverfunc.h"
#include "cardgamemsg.h"
#include "cardgame.h"
#include "../base/log.h"

RoomMgr::RoomMgr()
{
}

RoomMgr::~RoomMgr()
{
}

Room * RoomMgr::CreateRoom()
{
    Room * pRoom = new Room();
    if (pRoom)
    {
        m_IDMapRoom[pRoom->GetRoomID()] = pRoom;
        WriteLog(LEVEL_DEBUG, "A new room has been created. RoomID=%d.\n", pRoom->GetRoomID());
    }
    return pRoom;
}
Room * RoomMgr::GetRoom(int nRoomID)
{
    Room * pRoom = NULL;
    map< int, Room * >::iterator it = m_IDMapRoom.find(nRoomID);
    if (it != m_IDMapRoom.end())
    {
        pRoom = it->second;
    }
    return pRoom;
}

void RoomMgr::SendRoomInfo(CardSlot & rSlot)
{
    MSG_CARDGAME_S2C_RoomInfo msg;
    msg.nRoomNumber = m_IDMapRoom.size();
    int nIndex = rSlot.m_nPage * cMAX_PAGE_ROOM_NUM;
    while (nIndex >= 0)
    {
        if (nIndex >= (int)(m_IDMapRoom.size()))
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
            map< int, Room * >::iterator it = m_IDMapRoom.begin();
            for (int i=0; i<nIndex; i++)
            {
                ++it;
            }
            for (unsigned int i=0; it!=m_IDMapRoom.end() && i<cMAX_PAGE_ROOM_NUM; i++, ++it)
            {
                Room * pRoom = it->second;
                if (pRoom)
                {
                    msg.aRoomInfo[i].nRoomID = pRoom->GetRoomID();
                    msg.aRoomInfo[i].nRoomIndex = nIndex + i;
                    msg.aRoomInfo[i].nPlayerCount = pRoom->GetPlayerNumber();
                }
            }
            break;
        }
    }
    msg.nCurrentPage = rSlot.m_nPage;
    ::SendMsg(msg, rSlot.m_nSlotIndex);
}
void RoomMgr::HeartBeat(unsigned int nIntervalMS)
{
    map< int, Room * >::iterator it = m_IDMapRoom.begin();
    for (; it!=m_IDMapRoom.end();)
    {
        Room * pRoom = it->second;
        if (pRoom && (!pRoom->IsValid()))
        {
            WriteLog("RoomMgr::HeartBeat. Remove Invalid Room(%d).\n", pRoom->GetRoomID());
            delete pRoom;
            pRoom = NULL;
            m_IDMapRoom.erase(it++);
        }
        else
        {
            ++it;
        }
    }
}

