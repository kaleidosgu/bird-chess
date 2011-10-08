
--创建登录界面
local strLogin = string.format("%s%s",g_UIcurDir ,"/Res/UILua/login.lua")
dofile(strLogin)
createTmpUI(strLogin)