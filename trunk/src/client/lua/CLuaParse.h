#pragma once
#include "lua.hpp"
#include <algorithm>
#include <list>
#include <string>

using namespace std;

#define REGFUNFORLUA(L)  if(find(m_listFunc.begin(),m_listFunc.end(),""#L"") == m_listFunc.end()){m_listFunc.push_back(""#L"");\
	lua_register(m_pLuaState,""#L"",Lua_##L);}else{;}

struct EntitySize
{
	string sName;
	int nIndex;
	int x;
	int y;
	int w;
	int h;
};
class CLuaParse
{
public:
	CLuaParse(void);
	~CLuaParse(void);
	bool initLua();
	void RegeditAllCLuaFunc();
	int LoadLuaFile(const char* name);
	bool DoLuaFile(char *name,int nIndex,void* pt);
	bool SetLuaGloaData(char* cName,void* vStar);
	bool IsAttrOpen(char* cAttrName);
	bool GetLuaStrByIndex(char* cName,int nIndex,string& str);
	bool GetLuaIntByIndex(char* cName,int nIndex,int& nOut);

private:
	lua_State* m_pLuaState;
	list<string> m_listFunc;
};
