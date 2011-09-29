#include "cardgame.h"
#include "cardgamemsg.h"
#include "../base/log.h"
#include "../base/ini.h"
#include "../base/md5.h"
#include "../base/assert.h"
#include "dbquery/createplayerdbquery.h"
#include "dbquery/cardgamedbquerytasktype.h"

using namespace base;

CCardGame::CCardGame() :
    CServerCirculator("CardGame")
{
    m_nMaxClient = 0;
    m_aCardSlot = NULL;
}

CCardGame::~CCardGame()
{
    if (m_aCardSlot)
    {
        delete [] m_aCardSlot;
        m_aCardSlot = NULL;
    }
}

bool CCardGame::Initialize()
{
    bool bRes = CServerCirculator::Initialize();
    if (bRes)
    {
        m_nMaxClient = GIni.GetInt(m_strServerName.c_str(), "MaxClient", 32);
        WriteLog("The max client of server(%s) is %u.\n", m_strServerName.c_str(), m_nMaxClient);

        m_aCardSlot = new CardSlot[m_nMaxClient];
        for (unsigned int i=0; i<m_nMaxClient; i++)
        {
            m_aCardSlot[i].m_nSlotIndex = i;
        }
    }

    if (bRes)
    {
        DBQueryFactory::Instance(bRes);
    }
    return bRes;
}

void CCardGame::Destory()
{
    DBQueryFactory::DestoryInstance();
    if (m_aCardSlot)
    {
        delete [] m_aCardSlot;
        m_aCardSlot = NULL;
    }
}

void CCardGame::Pump()
{
    CServerCirculator::Pump();
    Task * pTask = NULL;
    while(true)
    {
        pTask = DBQueryFactory::Instance().GetCompletedTask();
        if (pTask == NULL)
        {
            break;
        }
        else
        {
            ProcessDBQueryResult(*((DBQueryTask *)pTask));
            delete pTask;
            pTask = NULL;
        }
    }
}

void CCardGame::ProcessMsg(MSG_BASE & rMsg, unsigned int nSlotIndex)
{
    WriteLog(LEVEL_DEBUG, "ProcessMsg. SlotIndex =%d, MSGID = %d.\n", nSlotIndex, rMsg.nMsg);

    switch (rMsg.nMsg)
    {
        case MSGID_SYSTEM_ConnectSuccess:
            {
                WriteLog(LEVEL_DEBUG, "One client connnect to the server.(%u).\n", nSlotIndex);
            }
            break;
        case MSGID_CARDGAME_C2S_LoginRequest:
            {
                m_aCardSlot[nSlotIndex].m_State = CardSlot::State_Logining;

                MSG_CARDGAME_C2S_LoginRequest & rLoginRequestMsg = (MSG_CARDGAME_C2S_LoginRequest &)rMsg;

                LoginDBQuery * pDBQuery = new LoginDBQuery();
                pDBQuery->m_strUserName = rLoginRequestMsg.szUsername;
                string strPassword = rLoginRequestMsg.szPassword;
                pDBQuery->m_strPassword = MD5String(strPassword);
                long long nTaskID = DBQueryFactory::Instance().AddTask(CGDBQTT_LoginRequest, pDBQuery);
                m_TaskIDSlotMap[nTaskID] = &m_aCardSlot[nSlotIndex];
            }
            break;
        case MSGID_CARDGAME_C2S_NextRoomPage:
            {
                m_aCardSlot[nSlotIndex].m_nPage++;
                m_RoomMgr.SendRoomInfo(m_aCardSlot[nSlotIndex]);
            }
            break;
        case MSGID_CARDGAME_C2S_CreateRoom:
            {
                Player * pPlayer = m_aCardSlot[nSlotIndex].m_pPlayer;
                if (pPlayer == NULL)
                {
                    MSG_CARDGAME_S2C_CreateRoomResult createRoomResult;
                    createRoomResult.nResult = MSG_CARDGAME_S2C_CreateRoomResult::Result_PlayerNotExist;
                    pPlayer->SendMsg(createRoomResult);
                }
                else
                {
                    if (m_aCardSlot[nSlotIndex].m_State == CardSlot::State_InGame)
                    {
                        Room * pRoom = m_RoomMgr.CreateRoom();
                        MSG_CARDGAME_S2C_CreateRoomResult createRoomResult;
                        createRoomResult.nRoomID = pRoom->GetRoomID();
                        pPlayer->SendMsg(createRoomResult);

                        MSG_CARDGAME_S2C_EnterRoomResult enterRoomResultMsg;
                        enterRoomResultMsg.nResult = pPlayer->EnterRoom(pRoom);
                        pPlayer->SendMsg(enterRoomResultMsg);
                        if (enterRoomResultMsg.nResult == MSG_CARDGAME_S2C_EnterRoomResult::Result_Success)
                        {
                            if (pRoom)
                            {
                                enterRoomResultMsg.nRoomID = pRoom->GetRoomID();
                            }
                            m_aCardSlot[nSlotIndex].m_State = CardSlot::State_InRoom;
                        }
                        else
                        {
                            if (pRoom)
                            {
                                pRoom->SetInvalid();
                            }
                            WriteLog(LEVEL_ERROR, "Enter Room Failed.\n");
                        }
                    }
                    else
                    {
                        MSG_CARDGAME_S2C_CreateRoomResult createRoomResult;
                        createRoomResult.nResult = MSG_CARDGAME_S2C_CreateRoomResult::Result_PlayerIsNotInGame;
                        pPlayer->SendMsg(createRoomResult);
                    }
                }
            }
            break;
        case MSGID_CARDGAME_C2S_EnterRoom:
            {
                MSG_CARDGAME_S2C_EnterRoomResult enterRoomResultMsg;
                Player * pPlayer = m_aCardSlot[nSlotIndex].m_pPlayer;
                if (pPlayer)
                {
                    pPlayer->LeaveRoom();
                    MSG_CARDGAME_C2S_EnterRoom & rEnterRoomMsg = (MSG_CARDGAME_C2S_EnterRoom &)rMsg;
                    Room * pRoom = m_RoomMgr.GetRoom(rEnterRoomMsg.nRoomID);
                    if (pRoom)
                    {
                        enterRoomResultMsg.nResult = pPlayer->EnterRoom(pRoom);
                        if (enterRoomResultMsg.nResult == MSG_CARDGAME_S2C_EnterRoomResult::Result_Success)
                        {
                            enterRoomResultMsg.nRoomID = pRoom->GetRoomID();
                            m_aCardSlot[nSlotIndex].m_State = CardSlot::State_InRoom;
                        }
                        else
                        {
                            WriteLog(LEVEL_DEBUG, "Enter the room failed. Reason(%d).\n", enterRoomResultMsg.nResult);
                        }
                    }
                    else
                    {
                        enterRoomResultMsg.nResult = MSG_CARDGAME_S2C_EnterRoomResult::Result_RoomNotExist;
                        WriteLog(LEVEL_DEBUG, "The room is not exsit.\n");
                    }
                }
                else
                {
                    enterRoomResultMsg.nResult = MSG_CARDGAME_S2C_EnterRoomResult::Result_PlayerNotExist;
                }
                pPlayer->SendMsg(enterRoomResultMsg);
            }
            break;
        case MSGID_CARDGAME_C2S_LeaveRoom:
            {
                Player * pPlayer = m_aCardSlot[nSlotIndex].m_pPlayer;
                if (pPlayer)
                {
                    pPlayer->LeaveRoom();
                    MSG_CARDGAME_S2C_LeaveRoomResult leaveRoomResultMsg;
                    pPlayer->SendMsg(leaveRoomResultMsg);
                }
            }
            break;
        case MSGID_SYSTEM_Disconnect:
            {
                // if on query the player info
                map < long long, CardSlot * >::iterator it = m_TaskIDSlotMap.begin();
                for (; it!= m_TaskIDSlotMap.end();)
                {
                    if (it->second && it->second->m_nSlotIndex == (int)nSlotIndex)
                    {
                        m_TaskIDSlotMap.erase(it++);
                    }
                    else
                    {
                        ++it;
                    }
                }
                if (m_aCardSlot[nSlotIndex].m_State == CardSlot::State_InGame || m_aCardSlot[nSlotIndex].m_State == CardSlot::State_InRoom)
                {
                    // Player disconnect
                    Player * pPlayer = m_aCardSlot[nSlotIndex].m_pPlayer;
                    if (pPlayer)
                    {
                        pPlayer->LeaveRoom();
                        m_PlayerIDMapSlotIndex.erase(pPlayer->GetPlayerID());
                        WriteLog(LEVEL_DEBUG, "A Player (%d) Leave.\n", pPlayer->GetPlayerID());
                    }
                    delete m_aCardSlot[nSlotIndex].m_pPlayer;
                    m_aCardSlot[nSlotIndex].m_pPlayer = NULL;
                }
                m_aCardSlot[nSlotIndex].m_State = CardSlot::State_Free;
            }
            break;
        default:
            {
                Player * pPlayer = m_aCardSlot[nSlotIndex].m_pPlayer;
                if (pPlayer)
                {
                    pPlayer->ProcessMsg(rMsg);
                }
            }
            break;
    }
}

void CCardGame::HeartBeat(unsigned int nIntervalMS)
{
    //WriteLog("HeartBeat.\n");
    m_RoomMgr.HeartBeat(nIntervalMS);
}

void CCardGame::_PlayerLogin(LoginDBQuery & rLoginDBQuery, CardSlot * pSlot)
{
    if (pSlot)
    {
        // player info
        Player * pPlayer = new Player(*pSlot, rLoginDBQuery.m_nDBPlayerID, rLoginDBQuery.m_strDBPlayerName, rLoginDBQuery.m_nDBScore, rLoginDBQuery.m_nDBWin, rLoginDBQuery.m_nDBDogfall, rLoginDBQuery.m_nDBLose);
        pSlot->m_pPlayer = pPlayer;
        m_PlayerIDMapSlotIndex[rLoginDBQuery.m_nDBPlayerID] = pSlot->m_nSlotIndex;

        WriteLog(LEVEL_DEBUG, "A new Player(%d, %s, %d) Login.\n", pPlayer->GetPlayerID(), pPlayer->GetPlayerName().c_str(), pPlayer->GetScore());
        MSG_CARDGAME_S2C_PlayerInfo playerInfoMsg;
        playerInfoMsg.nPlayerID = rLoginDBQuery.m_nDBPlayerID;
        strncpy(playerInfoMsg.szPlayerName, rLoginDBQuery.m_strDBPlayerName.c_str(), cMAX_USERNAME_LEN - 1);
        playerInfoMsg.nScore = rLoginDBQuery.m_nDBScore;
        playerInfoMsg.nWin = rLoginDBQuery.m_nDBWin;
        playerInfoMsg.nDogfall = rLoginDBQuery.m_nDBDogfall;
        playerInfoMsg.nLose = rLoginDBQuery.m_nDBLose;
        pPlayer->SendMsg(playerInfoMsg);

        // room info
        m_RoomMgr.SendRoomInfo(*pSlot);

        m_aCardSlot[pSlot->m_nSlotIndex].m_State = CardSlot::State_InGame;
    }
    else
    {
        WriteLog(LEVEL_ERROR, "CCardGame::_PlayerLogin. pSlot is NULL.\n");
    }
}

void CCardGame::_DisconnectClient(CardSlot * pSlot, int nReason)
{
    if (pSlot)
    {
        m_SocketMgr.DisconnectClient(pSlot->m_nSlotIndex, nReason);
    }
    else
    {
        WriteLog(LEVEL_ERROR, "CCardGame::_DisconnectClient. pSlot is NULL.\n");
    }
}

void CCardGame::ProcessDBQueryResult(DBQueryTask & rDBQueryTask)
{
    WriteLog("CGameServer::ProcessDBQueryResult.\n");
    switch (rDBQueryTask.m_nDBQueryTaskType)
    {
        case CGDBQTT_LoginRequest:
            {
                map< long long, CardSlot * >::iterator it = m_TaskIDSlotMap.find(rDBQueryTask.GetTaskID());
                if (it != m_TaskIDSlotMap.end())
                {
                    const list <DBQuery * > & rDBQueryList = rDBQueryTask.GetDBQueryList();
                    if (rDBQueryList.size() == 1)
                    {
                        LoginDBQuery * pLoginDBQuery = (LoginDBQuery *)(*(rDBQueryList.begin()));
                        if (pLoginDBQuery)
                        {
                            if (pLoginDBQuery->IsUserExist())
                            {
                                CardSlot * pSlot = it->second;
                                if (pSlot)
                                {
                                    if (pLoginDBQuery->IsValidUser())
                                    {
                                        map< int, int>::iterator itPlayerID = m_PlayerIDMapSlotIndex.find(pLoginDBQuery->m_nDBPlayerID);
                                        if (itPlayerID == m_PlayerIDMapSlotIndex.end())
                                        {
                                            MSG_CARDGAME_S2C_LoginResult loginResultMsg;
                                            SendMsg(loginResultMsg, pSlot->m_nSlotIndex);
                                            // the player login
                                            _PlayerLogin(*pLoginDBQuery, it->second);
                                        }
                                        else
                                        {
                                            // need drop the client
                                            if (itPlayerID->second >= 0 && itPlayerID->second < m_nMaxClient)
                                            {
                                                CardSlot & rPlayerSlot = m_aCardSlot[itPlayerID->second];
                                                MSG_CARDGAME_S2C_KickOffPlayer kickOffPlayer;
                                                kickOffPlayer.nReason = MSG_CARDGAME_S2C_KickOffPlayer::Reason_Relogin;
                                                SendMsg(kickOffPlayer, rPlayerSlot.m_nSlotIndex);
                                                if (rPlayerSlot.m_pPlayer)
                                                {
                                                    rPlayerSlot.m_pPlayer->LeaveRoom();
                                                    MSG_CARDGAME_S2C_LeaveRoomResult leaveRoomResultMsg;
                                                    rPlayerSlot.m_pPlayer->SendMsg(leaveRoomResultMsg);
                                                }
                                            }

                                            // the player has been in game.
                                            MSG_CARDGAME_S2C_LoginResult loginResultMsg;
                                            loginResultMsg.nResult = MSG_CARDGAME_S2C_LoginResult::Result_Relogin;
                                            SendMsg(loginResultMsg, pSlot->m_nSlotIndex);
                                            WriteLog(LEVEL_DEBUG, "The player(%d) relogin the game.\n", pLoginDBQuery->m_nDBPlayerID);
                                        }
                                    }
                                    else
                                    {
                                        MSG_CARDGAME_S2C_LoginResult loginResultMsg;
                                        loginResultMsg.nResult = MSG_CARDGAME_S2C_LoginResult::Result_Failed;
                                        SendMsg(loginResultMsg, pSlot->m_nSlotIndex);
                                    }
                                }
                                else
                                {
                                    WriteLog(LEVEL_ERROR, "CCardGame::ProcessDBQueryResult. CGDBQTT_LoginRequest. pSlot is NULL.\n");
                                }
                            }
                            else
                            {
                                // not valid, then create the user
                                CreatePlayerDBQuery * pDBQuery = new CreatePlayerDBQuery();
                                pDBQuery->m_strUserName = pLoginDBQuery->m_strUserName;
                                pDBQuery->m_strPassword = pLoginDBQuery->m_strPassword;
                                long long nTaskID = DBQueryFactory::Instance().AddTask(CGDBQTT_CreatePlayer, pDBQuery);
                                m_TaskIDSlotMap[nTaskID] = it->second;
                            }
                        }
                        else
                        {
                            WriteLog(LEVEL_ERROR, "Dispose CGDBQTT_LoginRequest. The login dbquery is null.\n");
                            _DisconnectClient(it->second, 0);
                        }
                    }
                    else
                    {
                        WriteLog(LEVEL_ERROR, "The number of dbquery for loginrequest is not correct.\n");
                        _DisconnectClient(it->second, 0);
                    }
                    m_TaskIDSlotMap.erase(it);
                }
                else
                {
                    WriteLog(LEVEL_DEBUG, "The client has disconnected before get the player info.\n");
                }
            }
            break;
        case CGDBQTT_CreatePlayer:
            {
                map< long long, CardSlot * >::iterator it = m_TaskIDSlotMap.find(rDBQueryTask.GetTaskID());
                if (it != m_TaskIDSlotMap.end())
                {
                    const list <DBQuery * > & rDBQueryList = rDBQueryTask.GetDBQueryList();
                    if (rDBQueryList.size() == 1)
                    {
                        CreatePlayerDBQuery * pCreatePlayerDBQuery = (CreatePlayerDBQuery *)(*(rDBQueryList.begin()));
                        if (pCreatePlayerDBQuery)
                        {
                            // create player then login.
                            LoginDBQuery * pDBQuery = new LoginDBQuery();
                            pDBQuery->m_strUserName = pCreatePlayerDBQuery->m_strUserName;
                            pDBQuery->m_strPassword = pCreatePlayerDBQuery->m_strPassword;
                            long long nTaskID = DBQueryFactory::Instance().AddTask(CGDBQTT_LoginRequest, pDBQuery);
                            m_TaskIDSlotMap[nTaskID] = it->second;
                        }
                        else
                        {
                            WriteLog(LEVEL_ERROR, "Dispose CGDBQTT_LoginRequest. The createplayer dbquery is null.\n");
                            _DisconnectClient(it->second, 0);
                        }
                    }
                    else
                    {
                        WriteLog(LEVEL_ERROR, "The number of dbquery for createplayer is not correct.\n");
                        _DisconnectClient(it->second, 0);
                    }
                    m_TaskIDSlotMap.erase(it);
                }
                else
                {
                    WriteLog(LEVEL_DEBUG, "The client has disconnected before create the player.\n");
                }
            }
            break;
        default :
            break;
    }
}
