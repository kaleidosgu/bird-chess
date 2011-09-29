#include "UIGlobalExport.h"
#include "../ui/UIExportWndBase.h"
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