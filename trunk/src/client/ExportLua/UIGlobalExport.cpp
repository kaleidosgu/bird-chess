#include "UIGlobalExport.h"
#include "../../netclient/clientsocketMgr.h"
#include "../ui/UIExportWndBase.h"
#include "../../cardgame/cardgamemsg.h"
#include "../UIGround.h"
extern clinetnet::CClientSocketMgr g_CSM;
extern string G_IPString;
extern int G_nPort;
CUIExportWndBase* CreateBaseExportWnd( lua_State* pState )
{
	int nTop = lua_gettop( pState );
	CWndBase* pDeskTop = NULL;
	CUIExportWndBase* pWndBse = NULL;

	lua_getglobal(pState, "g_UIGlobal");
	lua_pushstring(pState, "ptDesk");
	lua_gettable(pState, -2);
	if (!lua_islightuserdata(pState, -1))
	{

	}
	else
	{
		pDeskTop = (CWndBase*)lua_touserdata(pState, -1);

		pWndBse = new CUIExportWndBase;
		pWndBse->Create(0,0,0,0,pDeskTop,0);
	}
	lua_settop( pState, nTop );

	//tolua_pushusertype( pState, (void*)pWndBse,"CUIExportWndBase" );

	nTop = lua_gettop( pState );
	return pWndBse;
}

bool CreateUI( lua_State* pState, const char* pFileName )
{
	bool bSucceed = false;

	const char* pchar = NULL;

	if ( lua_isstring( pState, -1 ) )
	{
 		//lua_getglobal(pState, "g_UIGlobal");
//  		CWndBase* result = NULL;
//  		lua_pushstring(pState, "ptDesk");
//  		lua_gettable(pState, -2);
//  		if (!lua_islightuserdata(pState, -1))
//  		{
//  			int breakha = 0;
//  		}
//  
//  		result = (CWndBase*)lua_touserdata(pState, -1);

		int nRes = luaL_dofile(pState, pFileName );
		if( nRes != 0 )
		{
			nRes = 3;
		}
	}
	else
	{

	}


	return bSucceed;
}
void Login2Server( const char* pName, const char* pPassword)
{	
	g_CSM.Connect(G_IPString,G_nPort);
	MSG_CARDGAME_C2S_LoginRequest msg;
	strcpy_s(msg.szUsername,cMAX_USERNAME_LEN,pName);
	strcpy_s(msg.szPassword,cMAX_PASSWORD_LEN,pPassword);
	g_CSM.SendMsg(msg);
}

unsigned long GetARGB( int a, int r, int g, int b )
{
	return ARGB( a, r, g, b );
}

void SendChatMsg( lua_State* pState, const char* pChatContent )
{
	std::string strContent = pChatContent;
	g_UIGround->BirdChat( strContent );
}