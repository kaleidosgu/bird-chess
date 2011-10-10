local function OnKeyDown(self,eKey)
	
end
--consoleAddText
local function OnUpdate(self,e)
	self.tickCount = self.tickCount + e
	--consoleAddText(tostring(self.tickCount))
	if self.tickCount > 0.0001 then
		self.tickCount = self.tickCount - 0.0001
		local x = self.ptReturnbse:GetClientPos().x
		local y = 0
		x = x + 1
		local idx = 0
		if x > 1000 then
			x = 1
		end
		if x > 361 then
			idx = x % 361
			if idx == 0 then
				idx = 1
			end
		else
			idx = x
		end
		y = g_SinAngle[idx] * 50
		self.ptReturnbse:MoveWindow( x ,self.bsey + y)
	end
end

local function regEvent( parent )
	parent:SetScript( "OnKeyDown",OnKeyDown )
	parent:SetScript( "OnUpdate",OnUpdate )
end

function createTmpUI(strFile)
	local pMain = basewnd.toObject(g_UIGlobal["ptMainClient"],"CWndBase")
	local ptReturn = CreateUI(strFile,true,pMain,1,400)
	if ptReturn ~= nil then
		local ptReturnbse = basewnd.toObject(ptReturn,"CWndBase")
		local pChatBird = CWndLoadPicture:new();
		pChatBird:Create(0,0,"res/pic/EntityPic.png",ptReturnbse,0,true,0,378,33,34);
		pChatBird:SetFlip(true,false,false);
		
		
		ptReturn.ptReturnbse = ptReturnbse
		ptReturn.tickCount = 0
		ptReturn.bsey = ptReturnbse:GetClientPos().y
		regEvent(ptReturn)
	end
	ptReturn.myindex = 1
end