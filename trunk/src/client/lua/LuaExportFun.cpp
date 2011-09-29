#include "LuaExportFun.h"
#include "CLuaParse.h"



int Lua_GetPicSizeFromLua(lua_State* pLua)
{
	string sName = lua_tostring(pLua,1);
	int nIndex = lua_tonumber(pLua,2);
	int x = lua_tonumber(pLua,3);
	int y = lua_tonumber(pLua,4);
	int w = lua_tonumber(pLua,5);
	int h = lua_tonumber(pLua,6);
	EntitySize* pBS = (EntitySize*) lua_touserdata(pLua,7);
	pBS->sName = sName;
	pBS->nIndex = nIndex;
	pBS->h = h;
	pBS->w = w;
	pBS->x = x;
	pBS->y = y;
	return 1;

}

// 
// void CreateBirdByType(int nType ,int nCount = 1) 
// {
// 	for(int i = 0 ; i < nCount;i++)
// 	{
// 		MonsterBase *pBird = EntityFactory::Instance().CreateBird(nType,G_AngryBird,MainPlayer);
// 		G_AngryBird.push_back(pBird);	
// 	}
// }

int Lua_CreateBirdFromLua(lua_State* pLua)
{
	int nType = lua_tonumber(pLua,1);
	int nCount = lua_tonumber(pLua,2);
	//CreateBirdByType(nType,nCount);

	return 1;
}

int Lua_TestInCPP(lua_State* pLua)
{
	int a;
	a=lua_tonumber(pLua,1);
	return 1;
}


void CLuaParse::RegeditAllCLuaFunc()
{
	REGFUNFORLUA(GetPicSizeFromLua);
	REGFUNFORLUA(CreateBirdFromLua);

}