local function OnKeyDown(self,eKey)
	local keyres = WND_RESULT_NO
	return keyres
end
local function OnMessage(self,nUIEvent,nDlgID)
	local keyres = WND_RESULT_NO
	if nUIEvent == 0 then
		if nDlgID == 115 then
			--consoleAddText("btnClick")
			--self.fly = (not self.fly)
			-- local bShow = self.pWndBirdChat:IsVisible()
			-- if bShow == true then
				-- self.pWndBirdChat:ShowWindow(false)
				-- keyres = WND_RESULT_YES
			-- else
				-- self.pWndBirdChat:ShowWindow(true)
				-- keyres = WND_RESULT_YES
				
				-- local bseEdit = basewnd.toObject(self.stEdit,"CWndBase")
				-- bseEdit:SetFocus(bseEdit)
			-- end
			-- self:PlayMusic(3)
			math.randomseed(os.time())
			local nRandom = math.random(1,4)
			nRandom = math.random(1,4)
			consoleAddText("random is = " .. nRandom)
		end
	elseif nUIEvent == WND_ONCHAR then
		if nDlgID == 333 then
			consoleAddText("btnClick")
			self.fly = (not self.fly)
			keyres = WND_RESULT_YES
		end
	end
	return keyres
end
--consoleAddText
local function OnUpdate(self,e)
	if self.playmusic == false then
		self.tickTime = self.tickTime + e
		if self.tickTime > 2 then
			-- consoleClear()
			consoleAddText(self.tickCount )
			local ltime = os.time(os.date("*t"))
			if ltime >= self.tmpTime then
				self:PlayMusic(3)
				self.playmusic = true
			end
			self.tickTime = 0
		end
	end
	if self.fly == true then
		self.tickCount = self.tickCount + e
		--consoleAddText(tostring(self.tickCount))
		if self.tickCount > 0.0001 then
			consoleClear()
			consoleAddText(self.tickCount )
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
end

local function regEvent( parent )
	parent:SetScript( "OnKeyDown",OnKeyDown )
	parent:SetScript( "OnUpdate",OnUpdate )
	parent:SetScript( "OnMessage",OnMessage )
end

function createTmpUI(strFile)
	local pMain = basewnd.toObject(g_UIGlobal["ptMainClient"],"CWndBase")
	local ptReturn = CreateUI(strFile,true,pMain,1,400)
	if ptReturn ~= nil then
		local ptReturnbse = basewnd.toObject(ptReturn,"CWndBase")
		local pChatBird = CWndButton:new();
		pChatBird:Create(0,0,0,0,"res/pic/EntityPic.png",ptReturnbse,115);		
		pChatBird:AddRes(0,378,33,33)
		pChatBird:AddRes(0,410,33,33)
		pChatBird:AddRes(0,505,33,33)
		pChatBird:AddRes(0,543,33,33)
		
		ptReturn.ptReturnbse = ptReturnbse
		ptReturn.tickCount = 0
		ptReturn.tickTime = 0
		ptReturn.playmusic = false
		ptReturn.bsey = ptReturnbse:GetClientPos().y
		regEvent(ptReturn)
		ptReturn.fly = false
		
		local pWndBirdChat = CWndBase:new()
		pWndBirdChat:Create(30,0,0,0,ptReturnbse,0)
		pWndBirdChat:ShowWindow(false)
		ptReturn.pWndBirdChat = pWndBirdChat
		
		local sbg = CWndLoadPicture:new()
		sbg:Create( 0, 0, "res/uilua/chatbird.PNG", pWndBirdChat, 0, false, 0,0,0,0 )
		
		local stEdit = CWndEdit:new();
		stEdit:CreateNoFont(0,0,200,28,pWndBirdChat,333)
		--bseEdit.m_bDrawBox = true
		ptReturn.stEdit = stEdit
		stEdit:SetNotifyParent(true)
		stEdit:SetText("")
		local tmpt = os.date("*t")
		-- tmpt.min = tmpt.min + 1
		ptReturn.tmpTime = os.time(tmpt)
		ptReturn.tmpTime = ptReturn.tmpTime + 20 * 60
	end
	ptReturn.myindex = 1
end