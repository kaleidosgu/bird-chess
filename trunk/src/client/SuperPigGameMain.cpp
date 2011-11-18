///////////////////////////////////////////////////[
// SuperPig Pic&Sound Resouce is get from AngryBirds
// CreateBy:NONO 2011-01-23
// mail:wrw9@163.com 
// ///////////////////////////////////////////////////

#include <WinSock2.h>
#include <windows.h>
#include <tolua++.h>
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
#include "SoundSystem.h"
#include "lua/CLuaParse.h"
#include "UILoginGame.h"
#include "UIGround.h"
#include "HGEDevice.h"
#include "WndBase.h"
#include "WndDesktop.h"
#include "WndStatic.h"
#include "WndEdit.h"
#include "WndLoadPicture.h"
#include ".\cn\GfxFont.h"
#include "ClientNetMsgProcess.h"
char g_chUICurDir[256] = {'/0'};
GfxFont* pGfxFont		= NULL;	// 普通模式
GfxFont* pBlodFont		= NULL;	// 粗体模式
GfxFont* pItalicFont	= NULL;	// 斜体模式
GfxFont* pNotAntialias	= NULL; // 非平滑模式
GfxFont* pAntialias		= NULL; // 平滑模式

void InitFont()
{

	pGfxFont		= new GfxFont("宋体",12,FALSE,FALSE,FALSE);// 宋书，非粗体，非斜体，非平滑
	pBlodFont		= new GfxFont("宋体",18,TRUE,FALSE,FALSE);// 宋书，粗体，非斜体，非平滑
	pItalicFont		= new GfxFont("黑体",26,TRUE,TRUE,FALSE);// 黑书，粗体，斜体，非平滑
	pNotAntialias	= new GfxFont("隶书",36,TRUE,FALSE,FALSE);// 隶书，粗体，非斜体，非平滑
	pAntialias		= new GfxFont("隶书",36,TRUE,FALSE,TRUE);// 隶书，粗体，非斜体，平滑

	pGfxFont->SetColor(0xFF00FFFF);		// 设置像素字体颜色
	pBlodFont->SetColor(0xFFFF0FF0);	// 设置像素字体颜色
	pItalicFont->SetColor(0xFF0FF0FF);	// 设置像素字体颜色
	pNotAntialias->SetColor(0xFFFFF00F);// 设置像素字体颜色
	pAntialias->SetColor(0xFF0FFF0F);	// 设置像素字体颜色
}


#pragma   comment(lib,   "ws2_32.lib ")
using namespace std;
hgeGUI				*gui;
string G_IPString = "127.0.0.1";
int G_nPort = 7753;
CLuaParse G_LuaConfig;
bool G_bDebug = false;
CWndStatic* m_pStatic;
CWndEdit* m_pEdit;
HTEXTURE	tex, bgtex;
hgeSprite	*bgspr1,*bgspr2,*bgspr3,*bgspr4,*bgspr5; //5层背景,牛吧
hgeSprite	*SprMouse,*SprBirdDead,*SprBirdDead2;//*SprBullet *spr 
vector<hgeSprite*> bgSprVec;
HGE* hge= 0;
CWndBase* m_pWnd = 0;
CWndDesktop* m_pDesktop = 0;
HGEDevice* m_pHgeDevice = 0;
hgeFont				*fnt;  //字体
hgeFont				*fnt_WinOrLost;

float G_bgLocateX = 4000; //下面几个变量都是为了华丽的背景
float G_bgLocateX2 = 0;
float G_bgLocateY = 0;
float G_LogicX = 0;
float G_LogicY = 0;

float G_OrderLogic = 0;
int G_nGameStat = ENUM_GAME_STOP;
clinetnet::CClientSocketMgr g_CSM;
CUILoginGame* g_UILoginGame;
CUIGround* g_UIGround;
TOLUA_API int  tolua_WndLibExport_open (lua_State* tolua_S);
#include "lua/UILuaState.h"
/////////////////function////////////////


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
	//g_UILoginGame->Update(dt);
	g_UIGround->Update(dt);

#ifdef _DEBUG
	if( m_pDesktop )
	{
		m_pDesktop->OnUpdate( dt );
		m_pDesktop->OnMouseInput();
	}
#endif

	float MouseX,MouseY;
	hge->Input_GetMousePos(&MouseX,&MouseY);
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

	MouseY += G_LogicY; 
	MouseX += G_LogicX; 

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

	//下面是按键触发测试
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

			}
			break;
		case HGEK_F2:
			{

			}
			break;
		case HGEK_F3:
			{
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
		case HGEK_ESCAPE:	
			return true;
	}
	CShowMessage::Instance().Update(dt);
	gui->Update(dt);
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
	
	for(int i=0;i<10;i++)
	{
		bgspr5->RenderEx(bgspr2->GetWidth()*i-G_bgLocateX2-G_LogicX,500-G_LogicY,0.0,0.6,0.6);
		
	}
	//g_UILoginGame->Render();

	g_UIGround->Render();
#ifdef _DEBUG
	m_pDesktop->OnDraw();
#endif

	gui->Render();
	CShowMessage::Instance().Render();



	const char* lpString = ""
		"演示中文显示新方案，支持平滑(无锯齿)模式\n"
		"\n"
		"不需要依赖任何图片字模，丰富多样的显示方法。\n"
		"\n"
		"使用方法很简单，请参阅 <CN> 目录下的 Readme.txt 文件。\n";

	// 使用像素字体输出
	pGfxFont->Print(10,10,lpString);

	pBlodFont->Print(10,100,"中文显示方案 演示“粗体”");		
	pItalicFont->Print(10,150,"中文显示方案 演示“斜体”");
	pNotAntialias->Print(10,200,"中文显示方案 演示“非平滑模式”");
	pAntialias->Print(10,250,"中文显示方案 演示“平滑反锯齿模式”");

	m_pHgeDevice->hge->Gfx_EndScene();
	hge->Gfx_EndScene();

	return false;
}

void initCreateBirdLua()
{
	G_LuaConfig.initLua();
	G_LuaConfig.LoadLuaFile("Res\\config\\CreateBirdConfig.lua");
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
	InitFont();
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

	g_UIGround = new CUIGround();
	// 主角是一只猪
	SprMouse = EntityFactory::Instance().CreateSprite("SprMouse");
	SprMouse->SetHotSpot(0,0);
	//声音
	SoundSystem::init(hge);
#ifndef _DEBUG
	SoundSystem::Instance().PlayMusic(1); //formusic
#endif

	bgspr1=new hgeSprite(bgtex,0,0,4800,1200);
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
	delete SprBirdDead,SprBirdDead2;

	hge->Texture_Free(tex);
	hge->Texture_Free(bgtex);
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	m_pHgeDevice = new HGEDevice;
	CWndBase::SetDevice( m_pHgeDevice );
	m_pHgeDevice->hge = hgeCreate(HGE_VERSION);
	hge = m_pHgeDevice->hge;
	// Set desired system states and initialize HGE
	hge->System_SetState(HGE_LOGFILE, "SuperPig.log");
	hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
	hge->System_SetState(HGE_RENDERFUNC, RenderFunc);
	hge->System_SetState(HGE_TITLE, "SuperPigChess");
	//hge->System_SetState(HGE_FPS, GAME_FPS);
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
