local pDesk = basewnd.toObject(g_UIGlobal["ptDesk"],"CWndBase")
local ptMainClient = CWndBase:new();
ptMainClient:Create(0,0,800,600,pDesk,0);
g_UIGlobal["ptMainClient"] = ptMainClient