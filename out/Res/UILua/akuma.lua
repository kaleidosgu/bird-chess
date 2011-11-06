local function OnKeyDown(self,eKey)
	local keyres = WND_RESULT_NO
	-- consoleAddText(eKey)
	if eKey == 39 then
		self.sbg:MoveWindow(self.sbg:GetClientPos().x + 5,self.sbg:GetClientPos().y)
	elseif eKey == 37 then
		self.sbg:MoveWindow(self.sbg:GetClientPos().x - 5,self.sbg:GetClientPos().y)
	end
	if self.bgX < (self.sbg:GetClientPos().x - 150) then
		self.fakeUpdate = true
		self.fake1:ShowWindow(true)
	end
	if self.bgX1 < (self.sbg:GetClientPos().x - 200 ) then
		self.fakeUpdate1 = true
		self.fake2:ShowWindow(true)
	end
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
	if self.tickcounts > 0.03 then
		if self.fakeUpdate == true then
			if self.bgX >= self.sbg:GetClientPos().x  then
				self.fakeUpdate = false
				self.fake1:ShowWindow(false)
			end
			local ndiff = 5
			self.fake1:MoveWindow(self.fake1:GetClientPos().x + ndiff, self.fake1:GetClientPos().y)
			self.bgX = self.fake1:GetClientPos().x			
		end
		if self.fakeUpdate1 == true then
			if self.bgX1 >= self.sbg:GetClientPos().x  then
				self.fakeUpdate1 = false
				self.fake2:ShowWindow(false)
			end
			local ndiff = 5
			self.fake2:MoveWindow(self.fake2:GetClientPos().x + ndiff, self.fake2:GetClientPos().y)
			self.bgX1 = self.fake2:GetClientPos().x			
		end
		self.tickcounts = self.tickcounts - 0.03
	end
	self.tickcounts = self.tickcounts + e
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
		
		local fake2 = CWndLoadPicture:new()
		fake2:Create( 0, 400, "res/uilua/akuma.PNG", ptReturn, 0, false, 0,0,0,0 )
		fake2:ShowWindow(false)
		fake2:SetBlendMode(2)
		fake2:SetColor(GetARGB(200,0,100,255))
		ptReturn.fake2 = fake2
		
		local fake1 = CWndLoadPicture:new()
		fake1:Create( 0, 400, "res/uilua/akuma.PNG", ptReturn, 0, false, 0,0,0,0 )
		fake1:ShowWindow(false)
		fake1:SetBlendMode(2)
		fake1:SetColor(GetARGB(255,0,200,255))
		ptReturn.fake1 = fake1
		
		
		local sbg = CWndLoadPicture:new()
		sbg:Create( 0, 400, "res/uilua/akuma.PNG", ptReturn, 0, false, 0,0,0,0 )
		
		
		
		ptReturn.sbg = sbg
		
		local xpos = sbg:GetClientPos().x
		ptReturn.bgX = xpos
		ptReturn.bgX1 = xpos
		consoleAddText(tostring(xpos))
		
		ptReturn.fakeUpdate = false
		ptReturn.fakeUpdate1 = false
		
		ptReturn:SetFocus(nil)
		ptReturn.tickcounts = 0
		regEvent( ptReturn ) 
	end
end