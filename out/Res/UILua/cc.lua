function createTmpUI(strFile,simplestr)
	local pMain = basewnd.toObject(g_UIGlobal["ptMainClient"],"CWndBase")
	local ptReturn = CreateUI(strFile,true,pMain,1,400,simplestr)
	if ptReturn ~= nil then	
		ptReturn:ResizeWindow(300,300)
		ptReturn:ShowBox(true)
		
		-- local edit = CGfxEditWnd:new()
		-- edit:Create(30,30,100,15,ptReturn,0)
		-- edit:SetTextFont( 600, 0xFFFFFF00, "ËÎÌו");
		-- edit:ShowBox(true)
		
		
	end
end

-- local function OnKeyDown(self,eKey)
	-- local keyres = WND_RESULT_NO
	-- return keyres
-- end
-- local function focusOnEdit( self )
	-- self.pWndBirdChat:ShowWindow(true)
	-- self.stEdit:SetFocus(self.stEdit)
-- end
-- local function OnMessage(self,nUIEvent,nDlgID)
	-- local keyres = WND_RESULT_NO
	-- if nUIEvent == WND_CLICK then
		-- if nDlgID == 115 then
			-- -- self.fly = (not self.fly)
			-- local bShow = self.pWndBirdChat:IsVisible()
			-- if bShow == true then
				-- self.pWndBirdChat:ShowWindow(false)
				-- keyres = WND_RESULT_YES
				-- self.stEdit:SetFocus(nil)
				-- consoleAddText("false")
			-- else
				-- focusOnEdit( self )
				-- keyres = WND_RESULT_YES
				-- consoleAddText("true")
			-- end
		-- end
	-- elseif nUIEvent == WND_MOVEON then
		-- if nDlgID == 115 then
			-- self.fly = (not self.fly)
		-- end
	-- elseif nUIEvent == WND_MOVEOUT then
		-- if nDlgID == 115 then
			-- self.pWndBirdChat:ShowWindow(false)
			-- self.fly = (not self.fly)
		-- end
	-- elseif nUIEvent == WND_ONCHAR then
		-- if nDlgID == 333 then
-- --~ 			SendChatMsg(self.stEdit:GetText())
			-- self.pWndBirdChat:ShowWindow(false)
			-- self.fly = (not self.fly)
			-- keyres = WND_RESULT_YES
			-- self.stEdit:SetFocus(nil)
		-- end
	-- end
	-- return keyres
-- end
-- --consoleAddText
-- local function OnUpdate(self,e)
	-- if self.fly == true then
		-- self.tickCount = self.tickCount + e
		-- if self.tickCount > 0.01 then
			-- --consoleClear()
			-- self.tickCount = self.tickCount - 0.01
			-- local x = self.ptReturn:GetClientPos().x
			-- local y = 0
			-- local nRandy = 1
			-- x = x - 3
			-- local idx = 0
			-- if x < 1 then
				-- x = 1300

				-- if self.stEdit:GetText()~= "" then
					-- SendChatMsg(self.stEdit:GetText())
					-- self.stEdit:SetFocus(self.stEdit)
					-- self.stEdit:SetText("")
				-- end
			-- end
			-- if x > 361 then
				-- idx = x % 361
				-- if idx == 0 then
					-- nRandy = math.random(1,5)
					-- idx = 1
				-- end
			-- else
				-- idx = x
			-- end
			-- y = g_SinAngle[idx] *5*nRandy
			-- --self.ptReturn:MoveWindow( x ,self.bsey + y)
			-- self.ptReturn:MoveWindow( x ,700 + y)
			-- -- consoleAddText( " x = " .. x )
			-- self.zoom = self.zoom + 0.1
			-- if self.zoom > 1 then
				-- self.zoom = 0.2
			-- end
			-- self.sbg:SetZoom( self.zoom,self.zoom )
		-- end
	-- end
-- end

-- local function regEvent( parent )
	-- parent:SetScript( "OnKeyDown",OnKeyDown )
	-- parent:SetScript( "OnUpdate",OnUpdate )
	-- parent:SetScript( "OnMessage",OnMessage )
-- end
-- local function createBird( ptReturn )
	-- local pChatBird = CWndButton:new();
	-- pChatBird:Create(0,0,60,40,"res/pic/EntityPic.png",ptReturn,115);
	-- pChatBird:AddRes(0,378,33,33)
	-- pChatBird:AddRes(0,410,33,33)
	-- pChatBird:AddRes(0,505,33,33)
	-- pChatBird:AddRes(0,543,33,33)
	-- pChatBird:ShowBox(true)

	-- ptReturn.ptReturn = ptReturn
	-- ptReturn.tickCount = 0
	-- ptReturn.bsey = ptReturn:GetClientPos().y
	-- regEvent(ptReturn)
	-- ptReturn.fly = true

	-- local pWndBirdChat = CWndBase:new()
	-- pWndBirdChat:Create(30,0,0,0,ptReturn,0)
	-- -- pWndBirdChat:ShowWindow(false)
	-- ptReturn.pWndBirdChat = pWndBirdChat

	-- for i = 1, 1 do
		-- local sbg = CWndLoadPicture:new()
		-- sbg:Create( 15 + i * 40 , -30 * i + ( i * 30), "res/pic/EntityPic.png", pWndBirdChat, 0, true, 165,277,130,124 )
		-- sbg:SetZoom( 0.2, 0.2)
		-- ptReturn.sbg = sbg
	-- end

	-- local stEdit = CWndEdit:new();
	-- stEdit:CreateNoFont(15,0,200,28,pWndBirdChat,333)
	-- ptReturn.stEdit = stEdit
	-- -- pWndBirdChat:ShowWindow(false)
	-- stEdit:SetNotifyParent(true)
	-- stEdit:SetText("")
	-- stEdit:SetCaretColor(0xff000000)
	-- -- stEdit:ShowBox(true)
-- end
-- function createTmpUI(strFile,strSimple)
	-- local pMain = basewnd.toObject(g_UIGlobal["ptMainClient"],"CWndBase")
	-- local ptReturn = CreateUI(strFile,true,pMain,1,400,strSimple)
	-- if ptReturn ~= nil then
		-- createBird( ptReturn )
		-- ptReturn.zoom = 0.2
	-- end
-- end
-- createTmpUI("res\uilua\chatbird.lua","chatbird")
