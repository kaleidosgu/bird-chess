//////////////////////////////////////////////////////////////////////////
//
//
//			网络消息的处理
//
//
//
//
////////////////////////////////////////////////////////////////////////////

#include "../netclient/ClientSocketMgr.h"
#include "../cardgame/cardgamemsg.h"
extern int G_nGameStat;
extern CUIGround* g_UIGround;
extern clinetnet::CClientSocketMgr g_CSM;
extern lua_State*  g_pGlobalState;


#include "UIShowMessage.h"
#define UIShowMessage(X) CShowMessage::Instance().SetShowMesage(X)


void _ProcessMsg(MSG_BASE& rMsg) //
{

	//收消息,逻辑判断和处理
	switch (rMsg.nMsg)
	{
	case MSGID_CARDGAME_S2C_LoginResult:
		{
			MSG_CARDGAME_S2C_LoginResult& rLoginResultMsg  =  (MSG_CARDGAME_S2C_LoginResult&)rMsg;
			if(rLoginResultMsg.nResult == 0 ) //成功
			{
				lua_getglobal(g_pGlobalState,"hideLoginWnd");
				int nLuaRes = lua_isfunction(g_pGlobalState,-1);
				if( nLuaRes == 1)
				{
					nLuaRes = lua_pcall(g_pGlobalState,0,1,0);
					int nTop = lua_gettop( g_pGlobalState );
					if( nLuaRes == 0 )
					{
						lua_toboolean( g_pGlobalState, -1 );
					}
					else
					{
						const char* pChar = lua_tostring(g_pGlobalState, -1 );
						lua_settop(g_pGlobalState,0);
					}
					if( nTop > 0 )
					{
						const char* pChar = lua_tostring(g_pGlobalState, -1 );
						lua_settop(g_pGlobalState,0);
					}
				}

				SoundSystem::Instance().StopMusic(1);
				G_nGameStat = ENUM_CARDGAME_BEGIN;
				SoundSystem::Instance().PlayMusic(2);
				g_UIGround->SetReady(true);
			} 
			else //给个提示
			{
				UIShowMessage("Login fail!!");
			}
		}	
		break;
	case MSGID_CARDGAME_S2C_PlayerInfo:
		{
			MSG_CARDGAME_S2C_PlayerInfo& rPlayerInfoMsg  =  (MSG_CARDGAME_S2C_PlayerInfo&)rMsg;
			g_UIGround->SetMainPlayerInfo(rPlayerInfoMsg);
		}
		break;
	case MSGID_CARDGAME_S2C_RoomInfo:
		{
			//createroom
			MSG_CARDGAME_S2C_RoomInfo& rInfoMsg  =  (MSG_CARDGAME_S2C_RoomInfo&)rMsg;

			//这里后面修改,先测试一个房间的逻辑
			if(rInfoMsg.nRoomNumber == 0 )
			{

				MSG_CARDGAME_C2S_CreateRoom rRoomMsg;
				g_CSM.SendMsg(rRoomMsg);
			}
			else
			{
				bool bCreatRoom(true);
				for(int i =0 ;i < cMAX_PAGE_ROOM_NUM; ++i)
				{

					if( i >= rInfoMsg.nRoomNumber )
					{
						break;
					}
					if(rInfoMsg.aRoomInfo[i].nPlayerCount < 2)
					{
						MSG_CARDGAME_C2S_EnterRoom rRoomMsg;
						rRoomMsg.nRoomID = rInfoMsg.aRoomInfo[i].nRoomID;
						g_CSM.SendMsg(rRoomMsg);
						g_UIGround->SetRoomID(rRoomMsg.nRoomID);
						bCreatRoom = false;
						break;
					}
				}
				if(bCreatRoom)
				{

					MSG_CARDGAME_C2S_CreateRoom rRoomMsg;
					g_CSM.SendMsg(rRoomMsg);
				}
			}

		}
		break;
	case MSGID_CARDGAME_S2C_CreateRoomResult:
		{
			//提示成功或者失败
			MSG_CARDGAME_S2C_CreateRoomResult& rInfoMsg  =  (MSG_CARDGAME_S2C_CreateRoomResult&)rMsg;
			if(rInfoMsg.nResult != MSG_CARDGAME_S2C_CreateRoomResult::Result_Success )
			{
				UIShowMessage("CreateRoom fail!!");
			}
			else
			{
				g_UIGround->SetRoomID(rInfoMsg.nRoomID);
			}
		}
		break;
	case MSGID_CARDGAME_S2C_EnterRoomResult:
		{
			MSG_CARDGAME_S2C_EnterRoomResult& rInfoMsg  =  (MSG_CARDGAME_S2C_EnterRoomResult&)rMsg;
			if(rInfoMsg.nResult != MSG_CARDGAME_S2C_EnterRoomResult::Result_Success )
			{
				UIShowMessage("EnterRoomResult fail!!");
			}
		}
		break;
	case MSGID_CARDGAME_S2C_PlayerEnter:
		{
			//
			MSG_CARDGAME_S2C_PlayerEnter& rPlayerInfoMsg  =  (MSG_CARDGAME_S2C_PlayerEnter&)rMsg;
			g_UIGround->SetPlayerInfo(rPlayerInfoMsg);
		}
		break;
	case MSGID_CARDGAME_S2C_RoomMaster:
		{
			MSG_CARDGAME_S2C_RoomMaster& rPlayerInfoMsg  =  (MSG_CARDGAME_S2C_RoomMaster&)rMsg;
			g_UIGround->SetRoomMater(rPlayerInfoMsg.nPlayerID);
		}
		break;
	case MSGID_CARDGAME_S2C_PlayerLeave:
		{
			MSG_CARDGAME_S2C_PlayerLeave& rPlayerInfoMsg  =  (MSG_CARDGAME_S2C_PlayerLeave&)rMsg;
			g_UIGround->PlayerLeave(rPlayerInfoMsg.nPlayerID);

			//g_UIGround->SetRoomMater(rPlayerInfoMsg.nPlayerID);
		}
		break;
	case MSGID_CARDGAME_S2C_StartGame:
		{
			//开始成功
			UIShowMessage("Start!!");
			g_UIGround->Init();
			//g_UIGround->SetWinner( -1);
			SoundSystem::Instance().StopMusic(2);
			SoundSystem::Instance().PlaySound(38);
		}
		break;
	case MSGID_CARDGAME_S2C_StartGameFailed:
		{
			MSG_CARDGAME_S2C_StartGameFailed& rInfoMsg  =  (MSG_CARDGAME_S2C_StartGameFailed&)rMsg;
			UIShowMessage("StartGameFail fail!!");
			//开始失败
		}
		break;

	case MSGID_CARDGAME_S2C_SetCardType:
		{
			MSG_CARDGAME_S2C_SetCardType& rInfoMsg  =  (MSG_CARDGAME_S2C_SetCardType&)rMsg;
			g_UIGround->SetPlayerBirdID(rInfoMsg.nCardType);
		}
		break;
	case MSGID_CARDGAME_S2C_AddNewCard:
		{
			MSG_CARDGAME_S2C_AddNewCard& rInfoMsg  =  (MSG_CARDGAME_S2C_AddNewCard&)rMsg;
			g_UIGround->SetPlayerCardInfo(rInfoMsg.nCardPos,rInfoMsg.nCardType,rInfoMsg.nCardInstruction);
			/*unsigned int nCardPos; //主角身上的位置
			int nCardID;			//
			int nCardType;
			int nCardInstruction;*/
		}
		break;
	case MSGID_CARDGAME_S2C_CurrentTurnPlayer:
		{
			MSG_CARDGAME_S2C_CurrentTurnPlayer& rInfoMsg  =  (MSG_CARDGAME_S2C_CurrentTurnPlayer&)rMsg;
			g_UIGround->SetCurrentTurnPlayer(rInfoMsg.nPlayerID);
		}
		break;
	case MSGID_CARDGAME_S2C_UseCard:
		{
			MSG_CARDGAME_S2C_UseCard& rInfoMsg  =  (MSG_CARDGAME_S2C_UseCard&)rMsg;
			g_UIGround->Response(rInfoMsg.nCardType,rInfoMsg.nCardInstruction,rInfoMsg.nTargetCardType );
			/*nCardType = -1;
			nCardInstruction = 0;
			nTargetCardType = -1;*/
		}
		break;
	case MSGID_CARDGAME_S2C_UseCardFailed:
		{
			MSG_CARDGAME_S2C_UseCardFailed& rInfoMsg  =  (MSG_CARDGAME_S2C_UseCardFailed&)rMsg;
			if(rInfoMsg.nReason == MSG_CARDGAME_S2C_UseCardFailed::Reason_CardPosIsError)
			{
				UIShowMessage("UseCard Fail");
			}
			UIShowMessage("UseCard Fail");
		}
		break;
	case MSGID_CARDGAME_S2C_MoveCard:
		{

		}
		break;
	case MSGID_CARDGAME_S2C_Win:
		{

			MSG_CARDGAME_S2C_Win& rInfoMsg  =  (MSG_CARDGAME_S2C_Win&)rMsg;
			if( rInfoMsg.nPlayerID != -1)
			{
				//char temp[222];
				g_UIGround->SetWinner( rInfoMsg.nPlayerID );
				//sprintf_s(temp,"Player %d Win",rInfoMsg.nPlayerID );
				//UIShowMessage(temp);
			}	
			else
			{
				UIShowMessage("Nobody Win!!");
			}
			//编译不过。
			g_UIGround->SetPlayerBirdID(rInfoMsg.nOtherPlayerCardType,false);
			g_UIGround->SetGameState(false);
			//showmesage
		}
		break;

	}
}