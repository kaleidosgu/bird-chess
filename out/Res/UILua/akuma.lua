local function OnKeyDown(self,eKey)
	local keyres = WND_RESULT_NO
	consoleAddText(eKey)
	return keyres
end
local function OnMessage(self,nUIEvent,nDlgID)
	local keyres = 1
	if nUIEvent == 0 then

	end
	return keyres
end
--consoleAddText
local function OnUpdate(self,e)
	if self.playmusic == false then
		self.tickTime = self.tickTime + e
	end
end

local function regEvent( parent )
	parent:SetScript( "OnKeyDown",OnKeyDown )
	parent:SetScript( "OnUpdate",OnUpdate )
	parent:SetScript( "OnMessage",OnMessage )
end

function createTmpUI(strFile)
	local pMain = basewnd.toObject(g_UIGlobal["ptMainClient"],"CWndBase")
	local ptReturn = CreateUI(strFile,true,pMain,1,0)
	if ptReturn ~= nil then
		ptReturn.tickCount = 0
		ptReturn.tickTime = 0
		
		local sbg = CWndLoadPicture:new()
		sbg:Create( 0, 400, "res/uilua/akuma.PNG", ptReturn, 0, false, 0,0,0,0 )
		ptReturn:SetFocus(nil)
	end
end