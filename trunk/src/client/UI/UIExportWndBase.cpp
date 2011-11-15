#include "UIExportWndBase.h"
#include <lua.hpp>
#include <hge.h>
#include "UIDefine/UILuaScriptDefine.h"
#include "wnddatadefine.h"
#include "../SoundSystem.h"
CUIExportWndBase::CUIExportWndBase(void)
:m_pState(NULL),m_nCurTime(0)
{
	m_strPath = "";
}

CUIExportWndBase::~CUIExportWndBase(void)
{
}

bool CUIExportWndBase::Create( int x, int y, int cx, int cy, CWndBase* pParent, int nID )
{
	bool bRes = CWndBase::Create( x, y, cx, cy, pParent, 0 );
	return bRes;
}

void CUIExportWndBase::OnUpdate( float ft )
{
	CWndBase::OnUpdate( ft );

	int nIndexCall = 0;
	if( _IsUIEventExist(nIndexCall,UI_EVENT_ONUPDATE) )
	{
		const char* pChar = NULL;
		int nTop = lua_gettop( m_pState );
		lua_rawgeti( m_pState,LUA_REGISTRYINDEX,nIndexCall);
		int nRes = 0;

		tolua_pushusertype(m_pState,this,"CUIExportWndBase");
		lua_pushnumber( m_pState, ft );
		int nParam = 2;
		nRes = lua_pcall( m_pState, nParam, 1, 0);
		pChar = lua_tostring( m_pState, -1 );

		if( nRes > 0 )
		{
			lua_getglobal(m_pState,"consoleAddText");
			lua_pushstring( m_pState, pChar);
			nRes = lua_pcall( m_pState, 1,0,0);
		}
		lua_settop(m_pState,nTop);
		int ntop = lua_gettop( m_pState );
		nRes = 0;
	}
}

void CUIExportWndBase::SetScript( lua_State* pState, const char* pUIEvent, lua_Object nv)
{
	if( pUIEvent )
	{
		std::string tempEvent = pUIEvent;
		std::map < std::string, int >::iterator it = m_mapUIEventCall.find( tempEvent ) ;
		if( it == m_mapUIEventCall.end() )
		{
			m_mapUIEventCall[tempEvent] = luaL_ref( pState, LUA_REGISTRYINDEX );
		}
	}
	m_pState = pState;
}

int CUIExportWndBase::OnKeyDown( const hgeInputEvent& rEvent )
{
	int nReturn = WND_RESULT_NO;

	int nIndexCall = 0;
	if( _IsUIEventExist(nIndexCall,UI_EVENT_ONKEYDOWN) )
	{
		const char* pChar = NULL;
		int nTop = lua_gettop( m_pState );
		lua_rawgeti( m_pState,LUA_REGISTRYINDEX,nIndexCall );
		int nRes = 0;

		tolua_pushusertype(m_pState,this,"CUIExportWndBase");
		lua_pushnumber( m_pState, rEvent.key );
		int nParam = 2;
		nRes = lua_pcall( m_pState, nParam, 1, 0);
		pChar = lua_tostring( m_pState, -1 );
		int ntop = lua_gettop( m_pState );
		if( nRes == 0 && ntop == 1 )
		{
			nRes = lua_isnumber( m_pState, -1 );
			if( nRes == 1 )
			{
				nReturn = lua_tonumber( m_pState, -1 );
			}
		}
		else if ( nRes > 0 )
		{
			pChar = lua_tostring( m_pState, -1 );

			lua_getglobal(m_pState,"consoleAddText");
			lua_pushstring( m_pState, pChar);
			nRes = lua_pcall( m_pState, 1,0,0);
		}
		lua_settop(m_pState,nTop);
	}

	if( nReturn == WND_RESULT_NO )
	{
		nReturn = CWndBase::OnKeyDown( rEvent );
	}
	else
	{

	}
	return nReturn;
}

bool CUIExportWndBase::_IsUIEventExist( int& nCallIndex, const char* pChar )
{
	bool bResult = false;
	if( m_mapUIEventCall.size() > 0 && pChar )
	{
		std::string tempKey = pChar;
		std::map < std::string, int >::iterator it = m_mapUIEventCall.find( tempKey );
		if( it != m_mapUIEventCall.end() )
		{
			nCallIndex = it->second;
			if( nCallIndex > 0 )
			{
				bResult = true;
			}
		}
	}
	return bResult;
}
bool CUIExportWndBase::Destroy()
{
	for( std::map < std::string, int >::iterator it = m_mapUIEventCall.begin(); it != m_mapUIEventCall.end(); ++it )
	{
		int nRef = it->second;
		luaL_unref( m_pState, LUA_REGISTRYINDEX,nRef );
	}
	return CWndBase::Destroy();
}

int CUIExportWndBase::OnWndMessage( const int& nUIEvent, const int& nID )
{
	int nWndReturn = WND_RESULT_NO;
	int nIndexCall = 0;
	if( _IsUIEventExist(nIndexCall,UI_EVENT_OnMessage) )
	{
		const char* pChar = NULL;
		int nTop = lua_gettop( m_pState );
		lua_rawgeti( m_pState,LUA_REGISTRYINDEX,nIndexCall);
		int nRes = 0;

		tolua_pushusertype(m_pState,this,"CUIExportWndBase");
		lua_pushnumber( m_pState, nUIEvent );
		lua_pushnumber( m_pState, nID );
		int nParam = 3;
		nRes = lua_pcall( m_pState, nParam, 1, 0);
		pChar = lua_tostring( m_pState, -1 );

		int ntop = lua_gettop( m_pState );
		if( nRes > 0 )
		{
			lua_getglobal(m_pState,"consoleAddText");
			lua_pushstring( m_pState, pChar);
			nRes = lua_pcall( m_pState, 1,0,0);
		}
		else if( nRes == 0 && ntop == 1 )
		{
			nRes = lua_isnumber( m_pState, -1 );
			if( nRes == 1 )
			{
				nWndReturn = lua_tonumber( m_pState, -1 );
			}
		}
		lua_settop(m_pState,0);
	}
	if( nWndReturn == WND_RESULT_NO )
	{
		nWndReturn = CWndBase::OnWndMessage( nUIEvent, nID );
	}
	return nWndReturn;
}

void CUIExportWndBase::SetPath( const char* pChar )
{
	m_strPath = pChar;
}

void CUIExportWndBase::Playsound( int nID, int nPer )
{
	SoundSystem::Instance().PlaySound(nID,nPer);
}

void CUIExportWndBase::PlayMusic( int nID )
{
	SoundSystem::Instance().PlayMusic(nID);
}