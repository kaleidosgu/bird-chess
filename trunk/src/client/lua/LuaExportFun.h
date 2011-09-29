#pragma once

#include "./lua/lua.hpp"
#pragma comment(lib, "lua5.1.lib")
#pragma comment(lib, "lua51.lib")


int Lua_GetPicSizeFromLua(lua_State* pLua);
int Lua_CreateBirdFromLua(lua_State* pLua);
