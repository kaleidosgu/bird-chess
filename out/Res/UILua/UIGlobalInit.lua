local pDesk = basewnd.toObject(g_UIGlobal["ptDesk"],"CWndBase")
local ptMainClient = CWndBase:new();
ptMainClient:Create(0,0,800,600,pDesk,0);
g_UIGlobal["ptMainClient"] = ptMainClient


--初始化正弦，余弦值
g_CosAngle = {}
g_SinAngle = {}

for i = 1, 361 do
	local theta = (i - 1) * math.pi / 180
	g_CosAngle[i] = math.cos(theta) 
	g_SinAngle[i] = math.sin(theta) 
	--print( tostring(g_CosAngle[i] ) .. "==" .. tostring(g_SinAngle[i]) )
end