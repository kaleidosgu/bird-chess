///////////////////////////////////////////////////[
// SuperPig Pic&Sound Resouce is get from AngryBirds
// CreateBy:NONO 2011-01-23
// mail:wrw9@163.com 
// ///////////////////////////////////////////////////

#include <WinSock2.h>
#include <windows.h>
#include "../netclient/ClientSocketMgr.h"
#include "../cardgame/cardgamemsg.h"
#include "hge.h"
#include "hgefont.h"
#include "hgegui.h"
#include "hgeparticle.h"
#include "GameDefine.h"
#include <math.h>
#include <list>
#include <vector>
#include <string>
#include "EntityFactory.h"
#include "HeroPig.h"
#include "BulletBase.h"
#include "MonsterBase.h"
#include "SoundSystem.h"
#include "lua/CLuaParse.h"
#include "ActHeroPig.h"
#include "GEState.h"
#include "UILoginGame.h"
#include "UIGround.h"
#include "hgedevice/HGEDevice.h"
#include "wndlib/WndBase.h"
#include "wndlib/WndPicture.h"
#include "wndlib/WndDesktop.h"
#include "wndlib/WndStatic.h"
#include "wndlib/WndEdit.h"
#include "wndlib/WndLoadPicture.h"
#include <tolua++.h>
char g_chUICurDir[256] = {'/0'};


#include "UIShowMessage.h"
#define UIShowMessage(X) CShowMessage::Instance().SetShowMesage(X)

#pragma   comment(lib,   "ws2_32.lib ")
using namespace std;
//createbirdstrategy
int G_nMaxBirdCount = 0;
int G_nPerGroundCount =0;
int G_nPerGroundCreate =0;
string G_IPString = "127.0.0.1";
int G_nPort = 8888;
CLuaParse G_LuaConfig;
int G_nCombo = 0;
int G_nKillBird = 0;
int G_bgPrintType = ENUM_BG_NORMAL;
bool G_bDebug = false;
CWndStatic* m_pStatic;
CWndEdit* m_pEdit;
float m_ft = 0.0f;
HTEXTURE	tex, bgtex;
hgeSprite	*bgspr1,*bgspr2,*bgspr3,*bgspr4,*bgspr5; //5层背景,牛吧
hgeSprite	*SprMouse,*SprBirdDead,*SprBirdDead2;//*SprBullet *spr 
vector<hgeSprite*> bgSprVec;
hgeParticleSystem *BirdDead,*BirdDead2;
DWORD G_BirdDeadColor;
int m_ncount = 0;
//SuperPig

HeroPig* MainPlayer;
list<BulletBase*> G_Bulletlist;
list<MonsterBase*> G_AngryBird;
list<ItemBase*> G_ItemList;

HGE* hge= 0;
CWndBase* m_pWnd = 0;
CWndDesktop* m_pDesktop = 0;
CWndPicture* m_pPicture = 0;
CWndPicture* m_pPicture2 = 0;
HGEDevice* m_pHgeDevice = 0;

hgeFont				*fnt;  //字体
hgeFont				*fnt_WinOrLost;
hgeGUI				*gui;

int G_nScore = 0; //分数
int G_nGameStat = ENUM_GAME_STOP;//游戏状态
unsigned int G_nRound = 1;			//关卡回合
unsigned int G_NeedCreatrBirdsCount = BIRDS_COUNT ;
float G_fPassTime = 0.0;
float G_fTotalTime = 0.0;

float fDeadBirdX = SCREEN_WIDTH+100,fDeadBirdY =SCREEN_HEIGHT +100; //鸟毛特效的位置
bool G_bMouseDown =false;
float G_bgLocateX = 4000; //下面几个变量都是为了华丽的背景
float G_bgLocateX2 = 0;
float G_bgLocateY = 0;
float G_LogicX = 0;
float G_LogicY = 0;

float G_OrderLogic = 0;

clinetnet::CClientSocketMgr g_CSM;
CUILoginGame* g_UILoginGame;
CUIGround* g_UIGround;
TOLUA_API int  tolua_WndLibExport_open (lua_State* tolua_S);
#include "lua/UILuaState.h"
/////////////////function////////////////
void initGameData() 
{
	MainPlayer->SetLife(nDefaultLife);
	G_nScore = 0;
	G_nGameStat = ENUM_GAME_BEGIN;
	G_fPassTime = 0.0;
	G_nRound = 1;
	G_nCombo = 0;
	G_nKillBird = 0;
	G_NeedCreatrBirdsCount = BIRDS_COUNT;
	MainPlayer->SetPower(100);
	MainPlayer->SetBombCD(0);

	SoundSystem::Instance().StopMusic(1);
	
}

void ClearItem()
{
	list<ItemBase*>::iterator itBegItem = G_ItemList.begin();
	list<ItemBase*>::iterator itEndItem = G_ItemList.end();
	for(;itBegItem!= itEndItem;)
	{
		if((*itBegItem)->IsAlive())
		{
			delete *itBegItem;
			G_ItemList.remove(*itBegItem++);
		}
		else
			itBegItem++;
	}
}
void ClearBird()
{
	list<MonsterBase*>::iterator itBegBird = G_AngryBird.begin();
	list<MonsterBase*>::iterator itEndBird = G_AngryBird.end();
	for(;itBegBird!= itEndBird;)
	{
		delete *itBegBird;
		G_AngryBird.remove(*itBegBird++);
	}
}

void CreateBirdByType(int nType ,int nCount = 1) 
{
	for(int i = 0 ; i < nCount;i++)
	{
		MonsterBase *pBird = EntityFactory::Instance().CreateBird(nType,G_AngryBird,MainPlayer);
		G_AngryBird.push_back(pBird);	
	}
}


void CreateBird(int nCount ,int nRound) //产生鸟鸟
{
	for(int i = 0 ; i < nCount;i++)
	{
		MonsterBase* pBird = NULL;	
		if(nRound > 3 && i<1)
		{
				pBird = EntityFactory::Instance().CreateBird(ENUM_BIRDTYPE_CREATEEGGBIRD,G_AngryBird,MainPlayer);//我决定,先有鸡
				G_AngryBird.push_back(pBird);	
		}
// 
		if(nRound > 7 && i<1)
		{
			pBird = EntityFactory::Instance().CreateBird(ENUM_BIRDTYPE_SMALLBIRD,G_AngryBird,MainPlayer);//我决定,先有鸡
			G_AngryBird.push_back(pBird);	
		}
// 		
		int nRand = rand()%100;
		if(nRand>=60 || nRound == 1)//傻瓜鸟
		{
			pBird = EntityFactory::Instance().CreateBird(ENUM_BIRDTYPE_REDANGRYBIRD,G_AngryBird);
			//pBird = EntityFactory::Instance().CreateBird(ENUM_BIRDTYPE_SMARTANGRYBIRD,MainPlayer);
		}
		else if(nRand<20 && nRound >= 3) //冲锋
		{
			pBird = EntityFactory::Instance().CreateBird(ENUM_BIRDTYPE_RUSNANGRYBIRD,G_AngryBird,MainPlayer);
		}
		else if(nRand<40 && nRound >= 7) //肉盾
		{
			pBird = EntityFactory::Instance().CreateBird(ENUM_BIRDTYPE_STRONGANGRYBIRD,G_AngryBird);
		}
		else if(nRand<50 && nRound >= 9)
		{
			pBird = EntityFactory::Instance().CreateBird(ENUM_BIRDTYPE_SMARTANGRYBIRD,G_AngryBird,MainPlayer);
		}
		else
		{
			pBird = EntityFactory::Instance().CreateBird(ENUM_BIRDTYPE_REDANGRYBIRD,G_AngryBird);
		}
		G_AngryBird.push_back(pBird);	
	}
	//CreateBirdSnd();

}

void CreateBirdInLua(int nNeedCreatrBirdsCount,int nRound,int nAngryBirdSize)
{
	if(int(G_AngryBird.size()) < G_nPerGroundCount*nRound && nAngryBirdSize <= G_nMaxBirdCount)
	{
		G_NeedCreatrBirdsCount-=G_nPerGroundCreate+G_nRound;
		CreateBird(G_nPerGroundCreate+G_nRound,G_nRound);
		SoundSystem::Instance().PlaySound(13,30);
	}
}


void CreateItem(int ItemType = ENUM_ITEMTYPE_ADDLIFE)
{
	ItemBase* pItem = EntityFactory::Instance().CreateItem(ItemType,G_Bulletlist,G_AngryBird,MainPlayer);
	G_ItemList.push_back(pItem);	
	SoundSystem::Instance().PlaySound(14,70);
}

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
				//g_UILoginGame->SetEnable(false);
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

bool FrameFunc()  //帧数逻辑
{
	//网络消息的接收和发送
	while (true)
	{
		MSG_BASE * pMsg = g_CSM.GetMsg();
		if (pMsg)
		{
			_ProcessMsg(*pMsg);
			delete pMsg;
			pMsg = NULL;
		}
		else
		{
			break;
		}
	}


	//逻辑
	float dt=hge->Timer_GetDelta();
// 	m_ft += dt;
// 	if( m_pEdit )
// 	{
// 		char buffer[256] ={0};
// 		sprintf_s(buffer, "%d",(int)(m_ft * 1000 / 1000));
// 		m_pEdit->SetText(buffer);
// 	}
	gui->Update(dt);

	//g_UILoginGame->Update(dt);
	g_UIGround->Update(dt);

#ifdef _DEBUG
	if( m_pDesktop )
	{
		m_pDesktop->OnUpdate( dt );
		m_pDesktop->OnMouseInput();
	}
#endif

	G_fTotalTime += dt;
	if(G_nGameStat ==ENUM_GAME_BEGIN)
		G_fPassTime += dt;

	if(G_bgPrintType == ENUM_BG_BACK )
	{
		G_bgPrintType = ENUM_BG_NORMAL;
		for(int i=0;i<bgSprVec.size();i++)
		{
			bgSprVec[i]->SetBlendMode(BLEND_COLORMUL | BLEND_ALPHABLEND | BLEND_NOZWRITE);
		}
	}
	else if(G_bgPrintType == ENUM_BG_NEEDRESUME )
	{
		for(int i=0;i<bgSprVec.size();i++)
		{
			bgSprVec[i]->SetBlendMode(BLEND_COLORADD | BLEND_ALPHABLEND | BLEND_NOZWRITE);
		}
		G_bgPrintType = ENUM_BG_NORMAL;
	}

	float MouseX,MouseY;
	hge->Input_GetMousePos(&MouseX,&MouseY);

	//if((MouseY>500 || MouseY<268))// && (MainPlayer->GetY()<(1200-268) && MainPlayer->GetY()>268)) //Y轴滚屏
	//{
	//	if(MouseY>500 && G_LogicY<600 )
	//		G_LogicY++;
	//	else if(MouseY<268 && G_LogicY>0)
	//		G_LogicY--;
	//	g_UIGround->SetOrderY(G_LogicY);
	//}

	if((MouseX>1150 || MouseX<268)) //X轴滚屏
	{
		G_OrderLogic= 0;// 鼠标滚屏优先
		if(MouseX>1150 && G_LogicX<1300 )
		{
			if(MouseX<1240)
				G_LogicX+=3;
			else
				G_LogicX+=10;
		}
		else if (MouseX<268 && G_LogicX >0)
		{
			if(MouseX>100)
				G_LogicX-=3;
			else
				G_LogicX-=10;
		}
		g_UIGround->SetOrderX(G_LogicX);
	}


	if(G_OrderLogic!=0 && abs(G_OrderLogic -G_LogicX) > 100 )//自动平滑滚动,用全局来补救了,让真正的逻辑坐标接近目标逻辑坐标
	{
		int nSpeed(2);
		if( abs(G_OrderLogic -G_LogicX) > 800)
		{
			nSpeed = 16;
		}
		else if( abs(G_OrderLogic -G_LogicX) > 500)
		{
			nSpeed = 8;
		}
		if(G_OrderLogic -G_LogicX>0)
			G_LogicX += nSpeed;
		else
			G_LogicX -= nSpeed;;

		g_UIGround->SetOrderX(G_LogicX);
	}

	if (hge->Input_GetKeyState(HGEK_J)) 	
	{
		hge->Input_SetMousePos(MouseX-400*dt,MouseY);
	}
	if (hge->Input_GetKeyState(HGEK_L)) 
	{
		hge->Input_SetMousePos(MouseX+400*dt,MouseY);
		m_ncount++;
	}
	if (hge->Input_GetKeyState(HGEK_I)) hge->Input_SetMousePos(MouseX,MouseY-400*dt);
	if (hge->Input_GetKeyState(HGEK_K)) hge->Input_SetMousePos(MouseX,MouseY+400*dt);
	//MouseY += G_LogicY; 
	//MouseX += G_LogicX; 
	//MainPlayer->SetMousePosition(MouseX,MouseY);

	//x轴背景变化
	static float BgSpeed = 40;
	if(G_nGameStat == ENUM_GAME_BEGIN || G_nGameStat == ENUM_CARDGAME_BEGIN )
	{
		if(BgSpeed<150)
			BgSpeed+=40*dt;
	}
	else 
	{	
		if(BgSpeed>0 && G_nGameStat == ENUM_GAME_LOST)
			BgSpeed-=50*dt;
	}
	G_bgLocateX += dt*BgSpeed;
	if(G_bgLocateX>4800)
	{
		G_bgLocateX = 960;
	}

	G_bgLocateX2+= dt*BgSpeed*2;
	if(G_bgLocateX2>bgspr2->GetWidth()*5)
	{
		G_bgLocateX2 = 0;
	}

	//if((MainPlayer->GetRendY()>500 || MainPlayer->GetRendY()<268) && (MainPlayer->GetY()<(1200-268) && MainPlayer->GetY()>268)) //Y轴滚屏
	//{
	//	if(MainPlayer->GetRendY()>500 )
	//		G_LogicY=MainPlayer->GetY()-500;
	//	else
	//		G_LogicY=MainPlayer->GetY()-268;
	//	MainPlayer->SetOrderY(G_LogicY);
	//}

	//if((MainPlayer->GetRendX()>756 || MainPlayer->GetRendX()<268) && (MainPlayer->GetX()<(1200-268) && MainPlayer->GetX()>268)) //X轴滚屏
	//{
	//	if(MainPlayer->GetRendX()>756 )
	//		G_LogicX=MainPlayer->GetX()-756;
	//	else
	//		G_LogicX=MainPlayer->GetX()-268;
	//	MainPlayer->SetOrderX(G_LogicX);
	//}
	static float fButtleCD = 0.0;
	switch(hge->Input_GetKey())
	{
		//case HGEK_UP:		if(nObjects<MAX_OBJECTS) nObjects+=100; break;
		//case HGEK_DOWN:		if(nObjects>MIN_OBJECTS) nObjects-=100; break;
		case HGEK_SPACE:	
			{
				//MainPlayer->UsePower();
			}
			break;
		case HGEK_F1:
			{
				//g_UIGround->Response(1);
#ifdef _DEBUG
				/*MSG_CARDGAME_C2S_UseCard msg;
				msg.nCardID = 1;*/
				// TODO
			/*	if (m_nCard1Type == 5)
				{
					msg.nCardType = 0;
				}
				else
				{
					msg.nCardType = m_nCard1Type;
				}*/
				//g_CSM.SendMsg(msg);
				/*CreateBirdByType(ENUM_BIRDTYPE_EGGBIRD);
				CreateBirdByType(ENUM_BIRDTYPE_CREATEEGGBIRD);*/
// 				CreateItem(ENUM_ITEMTYPE_ADDSPEED);
// 				CreateItem(ENUM_ITEMTYPE_STOPALLBIRDS);
// 				CreateBirdByType(ENUM_BIRDTYPE_SMALLBIRD);
#endif			
			/*	CreateItem(rand()%(ENUM_ITEMTYPE_QUEENKISS-ENUM_ITEMTYPE_ADDLIFE) + ENUM_ITEMTYPE_ADDLIFE +1);
		
				CreateItem(rand()%(ENUM_ITEMTYPE_ADDLIFE-ENUM_ITEMTYPE_ADDSPEED) + ENUM_ITEMTYPE_ADDSPEED +1);
				CreateItem(ENUM_ITEMTYPE_STOPALLBIRDS);
				CreateItem(ENUM_ITEMTYPE_BECOMEBIRD);
				CreateItem(ENUM_ITEMTYPE_ADDLIFE);
				CreateItem(ENUM_ITEMTYPE_REALPIG);*/
			}
			break;
		case HGEK_F2:
			{
#ifdef _DEBUG
				//g_UIGround->Response(2);
#endif
			}
			break;
		case HGEK_F3:
			{
				//g_UIGround->Response(3);
#ifdef _DEBUG
				G_bDebug = !G_bDebug;
#endif
			}
			break;
		case HGEK_F4:
			{
				//g_UIGround->Response(4);
			}
			break;
		case HGEK_F5:
			{
				//g_UIGround->Response(5);
			}
			break;
		case HGEK_F6:
			{
				//g_UIGround->Response(6);
			}
			break;
		case HGEK_F7:
			{
				//g_UIGround->Response(7);
			}
			break;
		case HGEK_F8:
			{
				//g_UIGround->Response(8);
			}
			break;
		case HGEK_F9:
			{
				//g_UIGround->Response(9);
			}
			break;
		case HGEK_F10:
			{
				//g_UIGround->Response(10);
			}
			break;
		case HGEK_LBUTTON: 
			{
				//CreatBullet(rotion,(MouseY-y),(MouseX-x));
				//人物大招数,放屁
			}
			break;
		case HGEK_RBUTTON:  
			{
				//if(G_nGameStat != ENUM_GAME_BEGIN)
				//{
				//	initGameData();
				//	CreateBird(25,G_nRound);
				//}
				//else 
				//{
				//	BulletBase* pBullet;
				//	if(MainPlayer->GetBombCD() <=0)
				//	{
				//		for(int i = 0 ;i<10 ;i++)
				//		{
				//			pBullet =MainPlayer->UseBomb(i);
				//			if(pBullet)
				//			{
				//				 G_Bulletlist.push_back(pBullet);
				//			}
				//		}
				//		if(pBullet)
				//		{
				//			SoundSystem::Instance().PlaySound(23,70);
				//		}
				//		MainPlayer->SetBombCD(60);
				//	}
				//}
			}
			break;
		case HGEK_U:
			{
				//MainPlayer->SetRation(MainPlayer->GetBulletRation()+1);
			}
			break;
		case HGEK_I:
			{
				//MainPlayer->SetRation(MainPlayer->GetBulletRation()-1);
			}
			break;
		case HGEK_ESCAPE:	return true;
	}

	if(hge->Input_KeyDown(HGEK_N)||hge->Input_KeyDown(HGEK_LBUTTON)||hge->Input_KeyDown(HGEK_Q))
	{
		G_bMouseDown = true;
	}

	if( hge->Input_KeyDown(HGEK_F1))
	{
	}
	if(hge->Input_KeyUp(HGEK_N)||hge->Input_KeyUp(HGEK_LBUTTON)  )
	{
		G_bMouseDown = false;
	}
	//人物移动驱动

	static LastKeyState lastKey ;
	if(hge->Input_KeyDown(HGEK_A)) 
	{
		if(lastKey.IsRun(HGEK_A,G_fTotalTime))
		{
			CGEEvent State(EventID_StartRun,DirectionID_Left);
			((ActHeroPig*)MainPlayer)->Response(State);
		}
		else
		{
			CGEEvent State(EventID_StartWalk,DirectionID_Left);
			((ActHeroPig*)MainPlayer)->Response(State);
		}		
	}
	if(hge->Input_KeyUp(HGEK_A))
	{
		CGEEvent State(EventID_StopWalk,DirectionID_StopLeft);
		((ActHeroPig*)MainPlayer)->Response(State);		
	}
	if(hge->Input_KeyDown(HGEK_D)) 
	{
		if(lastKey.IsRun(HGEK_D,G_fTotalTime))
		{
			CGEEvent State(EventID_StartRun,DirectionID_Right);
			((ActHeroPig*)MainPlayer)->Response(State);
		}
		else
		{
			CGEEvent State(EventID_StartWalk,DirectionID_Right);
			((ActHeroPig*)MainPlayer)->Response(State);
		}		
	}
	if(hge->Input_KeyUp(HGEK_D))
	{
		CGEEvent State(EventID_StopWalk,DirectionID_StopRight);
		((ActHeroPig*)MainPlayer)->Response(State);		
	}


	if(hge->Input_KeyDown(HGEK_W)) 
	{
		if(lastKey.IsRun(HGEK_W,G_fTotalTime))
		{
			CGEEvent State(EventID_StartRun,DirectionID_Up);
			((ActHeroPig*)MainPlayer)->Response(State);
		}
		else
		{
			CGEEvent State(EventID_StartWalk,DirectionID_Up);
			((ActHeroPig*)MainPlayer)->Response(State);
		}		
	}
	if(hge->Input_KeyUp(HGEK_W))
	{
		CGEEvent State(EventID_StopWalk,DirectionID_StopUp);
		((ActHeroPig*)MainPlayer)->Response(State);		
	}

	if(hge->Input_KeyDown(HGEK_S)) 
	{
		if(lastKey.IsRun(HGEK_S,G_fTotalTime))
		{
			CGEEvent State(EventID_StartRun,DirectionID_Down);
			((ActHeroPig*)MainPlayer)->Response(State);
		}
		else
		{
			CGEEvent State(EventID_StartWalk,DirectionID_Down);
			((ActHeroPig*)MainPlayer)->Response(State);
		}		
	}
	if(hge->Input_KeyUp(HGEK_S))
	{
		CGEEvent State(EventID_StopWalk,DirectionID_StopDown);
		((ActHeroPig*)MainPlayer)->Response(State);		
	}

	if(hge->Input_KeyDown(HGEK_SPACE)) 
	{
		//CGEEvent State(EventID_StartJump);
		//((ActHeroPig*)MainPlayer)->Response(State);
	}

	//if (hge->Input_GetKeyState(HGEK_A)) MainPlayer->ChangeSpeed(-dt,0);
	// 	if (hge->Input_GetKeyState(HGEK_D)) MainPlayer->ChangeSpeed(dt,0);// dx+=MainPlayAddSpeed*dt;
	// 	if (hge->Input_GetKeyState(HGEK_W)) MainPlayer->ChangeSpeed(0,-dt);//dy-=MainPlayAddSpeed*dt;
	// 	if (hge->Input_GetKeyState(HGEK_S)) MainPlayer->ChangeSpeed(0,dt);//dy+=MainPlayAddSpeed*dt;


	//新的移动 没想好.....
	/*if (hge->Input_GetKeyState(HGEK_A)) MainPlayer->
	if (hge->Input_GetKeyState(HGEK_D)) 
	if (hge->Input_GetKeyState(HGEK_W)) 
	if (hge->Input_GetKeyState(HGEK_S)) */	
	MainPlayer->Update(dt);



	//射击判断
	//fButtleCD += dt;
	//if(G_bMouseDown && fButtleCD >= MainPlayer->GetShootCD())
	//{
	//	fButtleCD=0.0;
	//	G_Bulletlist.push_back(MainPlayer->CreateBullet(MouseX,MouseY));
	//	if(G_nGameStat == ENUM_GAME_BEGIN)
	//	{ 
	//		G_nScore-=5;
	//		G_nScore =max(0,G_nScore);
	//	}
	//}


	//刷怪策略
	static float  fCreatTime = 0;
	static float fCreateItem =0;
	fCreatTime += dt;
	fCreateItem += dt;
	if( fCreatTime > 1 && G_nGameStat == ENUM_GAME_BEGIN )
	{
		if(G_nRound>5 && fCreateItem>20) //道具策略 这个有空也搬到lua当中去nnd
		{
			fCreateItem = 0;
			CreateItem(rand()%3 + ENUM_ITEMTYPE_ADDSPEED);
		}
		if (G_NeedCreatrBirdsCount < 10*G_nRound && G_AngryBird.size()<10*G_nRound ) //游戏开始的时候创建鸟
		{
				G_nRound++;
				SoundSystem::Instance().PlaySound(12);
				if(G_nRound%2)
				{ 
					CreateItem(rand()%3 + ENUM_ITEMTYPE_BECOMEBIRD);//ENUM_ITEMTYPE_ADDLIFE);
				}
				CreateItem(rand()%3 + ENUM_ITEMTYPE_ADDSPEED);
				G_NeedCreatrBirdsCount += BIRDS_COUNT*G_nRound;
				fCreateItem = 0;
		}

		CreateBirdInLua(G_NeedCreatrBirdsCount,G_nRound,G_AngryBird.size());
		fCreatTime = 0;
	}

   //一个全局的特效,默认播放在很远的地方,有鸟死亡后会在鸟死亡的地方播放 ,这个可以移到鸟身上
	static float  fBirdFeatherTime = 0.0;
	fBirdFeatherTime += dt;
	if (fBirdFeatherTime >= 0.6)
	{
		BirdDead->FireAt(1222,800);
		BirdDead->Update(dt);
		BirdDead2->FireAt(1222,800);
		BirdDead2->Update(dt);
	}
	else
	{
		BirdDead->info.colColorStart = G_BirdDeadColor;
		BirdDead->info.colColorEnd = G_BirdDeadColor;
		BirdDead->FireAt(fDeadBirdX-G_LogicX+rand()%20 -10,fDeadBirdY-G_LogicY+rand()%20 -10);
		BirdDead2->FireAt(fDeadBirdX-G_LogicX+rand()%20 -10,fDeadBirdY-G_LogicY+rand()%20 -10);
		BirdDead->Update(dt);
		BirdDead2->Update(dt);
	}	

	//胜负判断
	if(G_nGameStat == ENUM_GAME_BEGIN && G_AngryBird.empty())
	{
		G_nGameStat = ENUM_GAME_WIN;
	}
	if(MainPlayer->GetLife() <= 0 && G_nGameStat ==ENUM_GAME_BEGIN)
	{
		G_nGameStat = ENUM_GAME_LOST;
		SoundSystem::Instance().PlaySound(11);
		ClearBird();
		ClearItem();
	}

	//以下是碰撞判定
	//鸟和人的判定
	//static float  TouchDelayTime = 0.0;
	//TouchDelayTime += dt;
	//if(TouchDelayTime > DMGDelayTime) {MainPlayer->GetCurrentSpr()->SetColor(0xFFFFFFFF);};
	//list<MonsterBase*>::iterator itBegBird = G_AngryBird.begin(); 
	//list<MonsterBase*>::iterator itEndBird = G_AngryBird.end();
	//for(;itBegBird!= itEndBird;)
	//{
	//	if((*itBegBird)->IsAlive())
	//	{
	//		(*itBegBird)->Update(dt);
	//		(*itBegBird)->MouseMoveStop(MouseX,MouseY);
	//		if((*itBegBird)->IsPlayerTouch(MainPlayer->GetX(),MainPlayer->GetY(),24)) 
	//		{
	//			if(TouchDelayTime > DMGDelayTime)
	//			{
	//				if(MainPlayer->GetPlayerStat() == ENUM_PLAY_NORMAL)
	//				{
	//					MainPlayer->SetLife(MainPlayer->GetLife()-1);
	//					TouchDelayTime = 0.0;
	//					if(MainPlayer->GetCurrentSpr())
	//						MainPlayer->GetCurrentSpr()->SetColor(0xFFFF0000);
	//					SoundSystem::Instance().PlaySound(15);
	//				}
	//				else if (MainPlayer->GetPlayerStat() == ENUM_PLAY_UNCONQUERABLE)
	//				{
	//					(*itBegBird)->SetDamage(10);
	//					G_nScore+=(*itBegBird)->GetScore();
	//				}
	//			}					
	//		}
	//		itBegBird++;
	//	}
	//	else
	//	{
	//		if((*itBegBird)->GetEntityStat() == ENUM_ENTITY_NEEDDESTORY)
	//		{	
	//			delete *itBegBird;
	//			G_AngryBird.remove(*itBegBird++);
	//		}
	//		else
	//		{
	//			if((*itBegBird)->GetEntityStat() == ENUM_ENTITY_NOTALIVE)
	//			{
	//				(*itBegBird)->OnDestory();
	//			}
	//			(*itBegBird)->Update(dt);
	//			itBegBird++;
	//		}
	//	}	
	//	
	//}


	//list<ItemBase*>::iterator itBegItem = G_ItemList.begin(); //物品和人的判定
	//list<ItemBase*>::iterator itEndItem = G_ItemList.end();
	//for(;itBegItem!= itEndItem;)
	//{
	//	(*itBegItem)->Update(dt);
	//	if((*itBegItem)->GetItemState() == ENUM_ITEM_TIMEOVER) //到时间了
	//	{	
	//		delete *itBegItem;
	//		G_ItemList.remove(*itBegItem++);
	//	}
	//	else if((*itBegItem)->IsAlive()) //还活着
	//	{
	//		if((*itBegItem)->IsPlayerTouch(MainPlayer->GetX(),MainPlayer->GetY(),24)) 
	//		{
	//			(*itBegItem)->ItemEffect();
	//			(*itBegItem)->SetEntityStat(ENUM_ENTITY_NOTALIVE);
	//		}
	//		itBegItem++;
	//	}
	//	else
	//	{
	//		if((*itBegItem)->GetEntityStat() == ENUM_ENTITY_NOTALIVE)//被吃掉了
	//		{
	//			(*itBegItem)->OnDestory();
	//		}
	//		itBegItem++;
	//	}
	//}

	////鸟和子弹的碰撞判定
	//list<BulletBase*>::iterator itBeg = G_Bulletlist.begin();
	//list<BulletBase*>::iterator itEnd = G_Bulletlist.end();
	//for(;itBeg!= itEnd;) 
	//{
	//	if((*itBeg)->IsAlive())
	//	{
	//		(*itBeg)->Update(dt);			
	//		if((*itBeg)->IsAlive())
	//		{
	//			list<MonsterBase*>::iterator itBegBird = G_AngryBird.begin();
	//			list<MonsterBase*>::iterator itEndBird = G_AngryBird.end();
	//			for(;itBegBird!= itEndBird;)
	//			{
	//				if((*itBegBird)->IsAlive())//
	//				{
	//					float distant = pow((*itBeg)->GetX() -(*itBegBird)->GetX() ,2)+ pow((*itBeg)->GetY() - (*itBegBird)->GetY(),2);
	//					float distant2 = pow((*itBeg)->GetWidth()*(*itBeg)->GetScale()/ 2+ (*itBegBird)->GetWidth()/2 ,2);

	//					if(  distant <= distant2) 
	//					{
	//						(*itBeg)->SetDamage(1);
	//						(*itBegBird)->SetDamage(1);
	//						if(!(*itBegBird)->IsAlive())
	//						{
	//								G_nScore += (*itBegBird)->GetScore();//一个鸟12分,一个子弹扣5分，如果两个子弹打掉你就够2的了
	//								G_nCombo++;
	//								G_nKillBird++;
	//								fDeadBirdX = (*itBegBird)->GetX(); //死亡位置给鸡毛用
	//								fDeadBirdY = (*itBegBird)->GetY();
	//								G_BirdDeadColor = (*itBegBird)->GetColor();
	//								fBirdFeatherTime = 0.0;//重置特效时间
	//								MainPlayer->SetPower(MainPlayer->GetPower()+1);
	//								//BirdDeadSound();
	//								SoundSystem::Instance().PlaySound(rand()%7,70);
	//						}
	//					}
	//				}
	//				itBegBird++;
	//			}
	//		}
	//		itBeg++;
	//	}
	//	else
	//	{
	//		delete *itBeg;
	//		G_Bulletlist.remove(*itBeg++);
	//	}
	//}
	CShowMessage::Instance().Update(dt);
	return false;
}


bool RenderFunc()
{

	// Render the scene
	hge->Gfx_BeginScene();
	bgspr1->Render(-G_bgLocateX-G_LogicX*.5,-G_LogicY*.5);
	bgspr1->Render(-G_bgLocateX-G_LogicX*.5+3840,-G_LogicY*.5);
	for(int i=0;i<60;i++)
	{
		bgspr3->Render(bgspr2->GetWidth()*i-G_bgLocateX2*.4-G_LogicX*.4,180-G_LogicY*.4);
		bgspr4->Render(bgspr2->GetWidth()*i-G_bgLocateX2*.6-G_LogicX*.6,200-G_LogicY*.4);
		bgspr2->Render(bgspr2->GetWidth()*i-G_bgLocateX2*.8-G_LogicX*.8,400-G_LogicY*.8);
	}
	
	////BirdDead->Render();
	//BirdDead2->Render();

	list<MonsterBase*>::iterator itBegBird = G_AngryBird.begin();
	list<MonsterBase*>::iterator itEndBird = G_AngryBird.end();
	for(;itBegBird!= itEndBird;itBegBird++)
	{
		(*itBegBird)->Render();
	}

	list<BulletBase*>::iterator itBeg = G_Bulletlist.begin();
	list<BulletBase*>::iterator itEnd = G_Bulletlist.end();
	for(;itBeg!= itEnd;itBeg++)
	{
		(*itBeg)->Render();
	}


	list<ItemBase*>::iterator itBegItem = G_ItemList.begin();
	list<ItemBase*>::iterator itEndItem = G_ItemList.end();
	for(;itBegItem!= itEndItem;itBegItem++)
	{
		(*itBegItem)->Render();
	}
	MainPlayer->Render();
	for(int i=0;i<10;i++)
	{
		//bgspr5->Render(bgspr2->GetWidth()*i-G_bgLocateX2-G_LogicX,500-G_LogicY);
		bgspr5->RenderEx(bgspr2->GetWidth()*i-G_bgLocateX2-G_LogicX,500-G_LogicY,0.0,0.6,0.6);
		
	}
	//g_UILoginGame->Render();

	g_UIGround->Render();
#ifdef _DEBUG
	m_pDesktop->OnDraw();
#endif
	gui->Render();
	//fnt->printf(7, 7, HGETEXT_LEFT, "Birds: %d\nRound: %d", G_AngryBird.size(),G_nRound);
	//fnt->printf(450,7,HGETEXT_LEFT,"Time: %6.2f",G_fPassTime);	
	//if(G_nCombo>=3)fnt->printf(1000,400,HGETEXT_RIGHT,"%d ComboHit",G_nCombo);
	//if( MainPlayer->GetPower() >= PLAYER_MAX_POWER)
	//	fnt->printf(1024-130,7,HGETEXT_LEFT,"Score: %d \nLife: %d\nPOWER: MAX!",G_nScore,MainPlayer->GetLife());	
	//else
	//	fnt->printf(1024-130,7,HGETEXT_LEFT,"Score: %d \nLife: %d\nPOWER: %d",G_nScore,MainPlayer->GetLife(),MainPlayer->GetPower());	

	//if(MainPlayer->GetBombCD() <= 0)
	//	fnt->printf(1024-130,7,HGETEXT_LEFT,"\n\n\nBOMB: READY!");
	//else
	//	fnt->printf(1024-130,7,HGETEXT_LEFT,"\n\n\nBOMBCD: %2.1f",MainPlayer->GetBombCD());
	//if(G_nGameStat == ENUM_GAME_STOP)
	//{
	//	//fnt_WinOrLost->printf(SCREEN_WIDTH/2,150, HGETEXT_CENTER,"[A,S,D,W] Move\n[LeftClick] Shoot\n[space] UsePower\n[RightClick] GO!");
	//}
	//if(G_nGameStat == ENUM_GAME_WIN)
	//{
	//	fnt_WinOrLost->SetColor(0xFFFF0000);
	//	fnt_WinOrLost->printf(450,300, HGETEXT_LEFT,"YOU WIN!\n\nScore: %d \nLife: %d\nPOWER: %d",G_nScore,MainPlayer->GetLife(),MainPlayer->GetPower());
	//}else if(G_nGameStat == ENUM_GAME_LOST)
	//{
	//	fnt_WinOrLost->SetColor(0xFFFF0000);
	//	fnt_WinOrLost->printf(SCREEN_WIDTH/2,250, HGETEXT_CENTER,"I WILL BE BACK!\nScore: %d \nKILL: %d \nTIME: %5.3f",G_nScore,G_nKillBird,G_fPassTime);
	//}
	//else if(G_nGameStat == ENUM_GAME_BEGIN)
	//{
	//	fnt_WinOrLost->SetColor(0xFF0000);
	//	fnt_WinOrLost->printf(7,300,HGETEXT_LEFT,"Begin GAME!!");
	//}
// 	m_pWnd->OnDraw();
// 	m_pWnd->DrawBox();

// 	m_pPicture->OnDraw();
// 
// 	m_pPicture2->OnDraw();

	CShowMessage::Instance().Render();
	m_pHgeDevice->hge->Gfx_EndScene();
	hge->Gfx_EndScene();

	return false;
}

void initCreateBirdLua()
{
	G_LuaConfig.initLua();
	G_LuaConfig.LoadLuaFile("Res\\config\\CreateBirdConfig.lua");
	G_LuaConfig.GetLuaIntByIndex("GetLuaInt",1,G_nMaxBirdCount);
	G_LuaConfig.GetLuaIntByIndex("GetLuaInt",2,G_nPerGroundCount); 
	G_LuaConfig.GetLuaIntByIndex("GetLuaInt",3,G_nPerGroundCreate);
	G_LuaConfig.GetLuaIntByIndex("GetLuaInt",4,G_nPort);
	G_LuaConfig.GetLuaStrByIndex("GetLuaStr",1,G_IPString);


//
	//这个luastate以后可能会移到合适的位置
	//////////////////////////////////////////////////////////////////////////
	g_pGlobalState = luaL_newstate();

	luaL_openlibs( g_pGlobalState );

	//导出库添加点
	int nRes = tolua_WndLibExport_open(g_pGlobalState);

	//////////////////////////////////////////////////////////////////////////
};
void LoadResource()
{
	initCreateBirdLua();
	g_CSM.Init(true, true);
	//bgtex=hge->Texture_Load("res/Pic/Bigbg1.png");
	bgtex=hge->Texture_Load("res/Pic/bg1.png");
	tex=hge->Texture_Load("res/Pic/EntityPic.png");
	HTEXTURE texUI=hge->Texture_Load("res/Pic/UILoginBG.png");

	fnt=new hgeFont("res/FontPsi/fontScore.fnt");
	fnt->SetScale(0.5);
	fnt_WinOrLost = new hgeFont("res/FontPsi/fontScore.fnt");
	EntityFactory::Instance().SetHTEXTURE(tex,texUI);

	//g_UILoginGame = new CUILoginGame(500,200,fnt);
	g_UIGround = new CUIGround();
   
	// 主角是一只猪

	MainPlayer = EntityFactory::Instance().CreatePlayer();
	SprMouse = EntityFactory::Instance().CreateSprite("SprMouse");
	//SprMouse->SetHotSpot(10,SprMouse->GetHeight());
	SprMouse->SetHotSpot(0,0);
	//SprMouse->SetHotSpot(10,15);
	//SprMouse =new hgeSprite(tex,16,474,20,29);
	//
	//SprBirdDead = new hgeSprite(tex,19,730,19,19);  //羽毛特效
	//SprBirdDead2 = new hgeSprite(tex,24,627,12,26);

	SprBirdDead = EntityFactory::Instance().CreateSprite("SprBirdDead");
	SprBirdDead2 = EntityFactory::Instance().CreateSprite("SprBirdDead2");
	BirdDead= new hgeParticleSystem("res/FontPsi/BirdDead.psi",SprBirdDead2);
	BirdDead2= new hgeParticleSystem("res/FontPsi/BirdDead2.psi",SprBirdDead);
	BirdDead->info.fSizeStart=0.3f;
	BirdDead->info.fSizeEnd=0.9f;
	BirdDead->TrackBoundingBox(true);

	//声音
	SoundSystem::init(hge);
#ifndef _DEBUG
	SoundSystem::Instance().PlayMusic(1); //formusic
#endif

	//背景
	bgspr1=new hgeSprite(bgtex,0,0,4800,1200);
	//bgspr1->SetZ(0.9);
	bgspr2=EntityFactory::Instance().CreateSprite("BGCloud2");//new hgeSprite(tex,47,0,480,50);
	bgspr3=EntityFactory::Instance().CreateSprite("BGCloud3");
	bgspr4=EntityFactory::Instance().CreateSprite("BGCloud4");
	bgspr5=EntityFactory::Instance().CreateSprite("BGCloud5");
	bgSprVec.push_back(bgspr1);
	bgSprVec.push_back(bgspr2);
	bgSprVec.push_back(bgspr3);
	bgSprVec.push_back(bgspr4);
	bgSprVec.push_back(bgspr5);

	for(int i=0;i<bgSprVec.size();i++)
	{
		bgSprVec[i]->SetBlendMode(BLEND_COLORADD | BLEND_ALPHABLEND | BLEND_NOZWRITE);
		bgSprVec[i]->SetColor(0xFF000000,0); //这种模式下后面这四个一定要有
		bgSprVec[i]->SetColor(0xFF000000,1);
		bgSprVec[i]->SetColor(0xFF000040,2);
		bgSprVec[i]->SetColor(0xFF000040,3);
	}
	

	// UI
	gui=new hgeGUI();
	gui->SetCursor(SprMouse);
}
void FreeResource()
{
	delete fnt;
	delete fnt_WinOrLost;
	//delete spr;
	delete bgspr1,bgspr2,bgspr3,bgspr4,bgspr5;
	delete SprMouse;
	delete SprBirdDead,SprBirdDead2,BirdDead,BirdDead2;

	hge->Texture_Free(tex);
	hge->Texture_Free(bgtex);
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	m_pHgeDevice = new HGEDevice;
	CWndBase::SetDevice( m_pHgeDevice );
	m_pHgeDevice->hge = hgeCreate(HGE_VERSION);
	hge = m_pHgeDevice->hge;

	hge = hgeCreate(HGE_VERSION);
	// Set desired system states and initialize HGE
	hge->System_SetState(HGE_LOGFILE, "SuperPig.log");
	hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
	hge->System_SetState(HGE_RENDERFUNC, RenderFunc);
	hge->System_SetState(HGE_TITLE, "SuperPigChess");
	hge->System_SetState(HGE_FPS, GAME_FPS);
	hge->System_SetState(HGE_USESOUND, true);
	hge->System_SetState(HGE_WINDOWED, true);
	//hge->System_SetState(HGE_WINDOWED, false);
	hge->System_SetState(HGE_SCREENWIDTH, 1280);
	hge->System_SetState(HGE_SCREENHEIGHT, 768);
	hge->System_SetState(HGE_SCREENBPP, 32);
	hge->System_SetState(HGE_SHOWSPLASH,false); 
	hge->System_SetState(HGE_DONTSUSPEND,true);
	//hge->System_SetState(HGE_ZBUFFER,true); 

	if(hge->System_Initiate())
	{


// 		m_pWnd = new CWndBase;
// 		m_pWnd->Create(0,0,100,100,0,0);


		LoadResource();

		m_pDesktop = new CWndDesktop;
		m_pDesktop->Create(0,0,1024,700,0,0);
		
		DWORD dw = GetCurrentDirectory(256,g_chUICurDir);

		//////////////////////////////////////////////////////////////////////////
		lua_newtable(g_pGlobalState); 
		lua_pushstring(g_pGlobalState, "ptDesk");
		tolua_pushusertype(g_pGlobalState,m_pDesktop,"CWndBase");
		lua_settable(g_pGlobalState, -3);
		lua_setglobal(g_pGlobalState,"g_UIGlobal");

		lua_pushstring(g_pGlobalState,g_chUICurDir);
		lua_setglobal(g_pGlobalState,"g_UIcurDir");
		
		const char* pChar = NULL;

		int nLuaRes = 0;
		//////////////////////////////////////////////////////////////////////////
		m_pDesktop->m_pHgeFont = fnt;

		
		nLuaRes = luaL_dofile( g_pGlobalState,"Res\\UILua\\UIGlobal.lua" );
		if ( nLuaRes > 0 )
		{
			pChar = lua_tostring(g_pGlobalState,-1);
		}
		if( g_pGlobalState )
		{
			nLuaRes = luaL_dofile(g_pGlobalState, "Res\\UILua\\console.lua");
			if( nLuaRes > 0 )
			{
				pChar = lua_tostring( g_pGlobalState, -1 );
			}
			else
			{
			}

			int nTop = lua_gettop( g_pGlobalState );

			lua_settop( g_pGlobalState, 0 );
			nLuaRes = 0;
		}

		nLuaRes = luaL_dofile( g_pGlobalState,"Res\\UILua\\UICreateProcess.lua" );
		if ( nLuaRes > 0 )
		{
			pChar = lua_tostring(g_pGlobalState,-1);
		}

// 		CWndLoadPicture* pPic = new CWndLoadPicture;
// 		pPic->Create(0,0,"res/UILua/ConsoleBG.png",m_pDesktop,0);
		//////////////////////////////////////////////////////////////////////////
		//下面是测试用的。不要删哟.
// 		m_pPicture = new CWndPicture;
// 		m_pPicture->Create(0,0,"StrongAngryBird",m_pDesktop,0);
// 
// 		m_pPicture2 = new CWndPicture;
// 		m_pPicture2->Create(40,0,"StrongAngryBird",m_pDesktop,0);
// 		m_pPicture2->MoveWindow(100,0);
// 		m_pPicture2->SetTextureInfo( 2 );


// 		m_pStatic = new CWndStatic;
// 		m_pStatic->Create(100,0,100,100,"haaaahab",m_pDesktop,0,fnt,HGETEXT_CENTER);

#ifdef _DEBUG
//   		m_pEdit = new CWndEdit;
//   		m_pEdit->Create(100,0,100,100,"abcdefghjk",m_pDesktop,0,fnt,HGETEXT_LEFT);
//  		m_pEdit->SetText("fa");
#endif
		//////////////////////////////////////////////////////////////////////////
		hge->System_Start();
		FreeResource();
		m_pDesktop->Destroy();
	}

	// Clean up and shutdown
	m_pHgeDevice->hge = NULL;
	hge->System_Shutdown();
	hge->Release();
	return 0;
}
