#include "CLuaParse.h"



CLuaParse::CLuaParse(void):m_pLuaState(NULL)
{
}

CLuaParse::~CLuaParse(void)
{
	if(m_pLuaState)
	{
		lua_close(m_pLuaState);
		m_pLuaState = NULL;
	}
}

bool CLuaParse::initLua()
{
	m_pLuaState = lua_open();
	if(!m_pLuaState )
	{
		return false;
	}
	luaL_openlibs(m_pLuaState);
	RegeditAllCLuaFunc();
	return true;
}

int CLuaParse::LoadLuaFile(const char* name)
{
	int nRes(0);
	//string str;
	//str ="E:\\Study\\SVN\\Nono\\tortoiseChess\\tortoiseChess\\tortoiseChess\\lua\\EntityConfig.lua";
	//str ="1";

	nRes = luaL_dofile(m_pLuaState,name);
	const char* pChar = lua_tostring(m_pLuaState,-1);
	return nRes;
}

bool CLuaParse::DoLuaFile(char *name,int nIndex,void* pt)
{
	lua_getglobal(m_pLuaState,name);
//	lua_getglobal(m_pLuaState,"add");

	SetLuaGloaData("curBirdPt" ,pt);

	int nRes = lua_isfunction(m_pLuaState,-1);
	if(nRes == false)
	{
		lua_settop(m_pLuaState,0);
		return false;
	}
	lua_pushinteger(m_pLuaState,nIndex);
	int res = lua_pcall(m_pLuaState,1,1,NULL);
	return res;
}

bool CLuaParse::SetLuaGloaData(char* cName,void* vStar)
{
	if(cName ==NULL)
	{
		return false;
	}
	lua_pushlightuserdata(m_pLuaState,(void*)vStar);
	lua_setglobal(m_pLuaState,cName);
	return true;
}
bool CLuaParse::IsAttrOpen(char* cAttrName)
{
	bool bSuccess = false;
	if( cAttrName&& m_pLuaState)
	{
		lua_getglobal(m_pLuaState,cAttrName);
		bool nRes = lua_isboolean(m_pLuaState,-1);
		if( nRes)
		{
			bSuccess = lua_toboolean(m_pLuaState,-1);
		}
	}
	return bSuccess;
}



bool CLuaParse::GetLuaStrByIndex(char* cName,int nIndex, string& str)
{
	bool bSucess = false;
	if(cName&&m_pLuaState)
	{
		lua_getglobal(m_pLuaState,cName);
		lua_pushinteger(m_pLuaState,nIndex);
		
		int nRes = lua_pcall(m_pLuaState,1,LUA_MULTRET,-1);
		if(nRes == 0)
		{
			if(lua_isstring(m_pLuaState,-1) == 1)
			{
				str = lua_tostring(m_pLuaState,-1);
				bSucess = true;
			}
		}
		else 
		{
			if(lua_isstring(m_pLuaState,-1) == 1)
			{
				str = lua_tostring(m_pLuaState,1);
				bSucess = false;
			}
		}
	}
	return bSucess;
}




bool CLuaParse::GetLuaIntByIndex(char* cName,int nIndex,int& nOut)
{
	bool bSucess = false;
	if(cName&&m_pLuaState)
	{
		lua_getglobal(m_pLuaState,cName);
		lua_pushinteger(m_pLuaState,nIndex);

		int nRes = lua_pcall(m_pLuaState,1,LUA_MULTRET,-1);
		if(nRes == 0)
		{
			if(lua_isnumber(m_pLuaState,-1) == 1)
			{
				nOut = lua_tonumber(m_pLuaState,-1);
				bSucess = true;
			}
			else
			{
				int p = lua_type(m_pLuaState,-1);
				p = NULL;
			}
		}
		else 
		{
			if(lua_isnumber(m_pLuaState,-1) == 1)
			{
				nOut = lua_tonumber(m_pLuaState,1);
				bSucess = false;
			}
		}
	}
	return bSucess;
}
