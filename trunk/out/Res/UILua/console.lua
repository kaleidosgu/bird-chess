local function OnUpdate(self,e)
	local kk = ""
	if self.m_bShowEdit then
		if self.m_nSpeed < 0 then
			self.m_nSpeed = self.m_nSpeed * (-1);
		end
	else
		if self.m_nSpeed > 0 then
			self.m_nSpeed = 0 - self.m_nSpeed
		end
	end
	local ptx = self.movewnd:GetClientPos().x
	local pty = self.movewnd:GetClientPos().y
	local szy = self.movewnd:GetClientSize().cy
	local bChanged = false
	if self.m_bShowEdit then
		if pty  < 0 then
			bChanged = true
		end
	else
		if pty >  0 - szy  then
			bChanged = true;
		end
	end

	if bChanged then
		self.movewnd:MoveWindow( ptx, pty + self.m_nSpeed )
		kk = "aa"
	end
	return kk
end

local function OnKeyDown(self,eKey)
	local str = "onkeydown"
	local keyres = WND_RESULT_NO
	if eKey == 112 then
		self.m_bShowEdit = (not self.m_bShowEdit)
		keyres = WND_RESULT_YES
	end
	return keyres
end

local function OnMessage(self,nUIEvent,nDlgID)
	local keyres = WND_RESULT_NO
	if nUIEvent == WND_ONCHAR then
		if nDlgID == 35 then
			if self.m_bShowEdit == true then
				local kdString = self.myedit:GetText()
				
				local firstStr = {}
				local ncount = 1
				for w in string.gmatch(kdString, "%a+") do
					firstStr[ncount] = tostring(w)
					ncount = ncount + 1;
				end
				if firstStr[1] == "create" then
					local ends = string.find(kdString," ")
					local substr = string.sub(kdString,ends + 1,string.len(kdString))
					substr = string.format("\\res\\uilua\\%s.lua",substr)
					local strFile = g_UIcurDir .. substr
					local bExist = IsFileExist(strFile)
					if bExist == true then
						--consoleAddText(strFile)
						dofile(strFile)
						createTmpUI(strFile)
					else
						consoleAddText(strFile .. " File not exist!")
					end
				elseif firstStr[1] == "cls" then
					consoleClear()
				end
				keyres = WND_RESULT_YES
			end
		end
	end
	return keyres
end

local function regEvent( parent )
	parent:SetScript( "OnUpdate",OnUpdate )
	parent:SetScript( "OnKeyDown",OnKeyDown )
	parent:SetScript( "OnMessage",OnMessage )
end

function consoleAddText( str )
	g_pConsole.context:AddText(str)
end
function consoleClear( )
	g_pConsole.context:SetText("")
end

local pDesk = basewnd.toObject(g_UIGlobal["ptDesk"],"CWndBase")
local expwnd = CreateLuaWnd(pDesk,0,0)
expwnd:SetPath("console")
expwnd.m_bShowEdit = true
expwnd.m_nSpeed = 30
local bseexp = basewnd.toObject(expwnd,"CWndBase")

local movewnd = CWndBase:new()
expwnd.movewnd = movewnd
movewnd:Create(0,0,0,0,bseexp,0)

local bg = CWndLoadPicture:new()
bg:Create( 0, 0, "res/UILua/ConsoleBG.png", movewnd, 0,false,0,0,0,0 );

local bsebg = basewnd.toObject(bg,"CWndBase")

local cx = bsebg:GetClientSize().cx
local cy = bsebg:GetClientSize().cy
movewnd:ResizeWindow(cx,cy)
movewnd:MoveWindow(0,0 - cy )

local stText = CWndStatic:new()
expwnd.context = stText
stText:CreateNoFont(0,0,100,28,"haha",movewnd,0)
stText:SetText("")

local stEdit = CWndEdit:new();
stEdit:CreateNoFont(0,280,100,28,movewnd,35)
stEdit:SetNotifyParent(true)
stEdit:SetText("")
stEdit:ShowBox(true)
expwnd.myedit = stEdit
local bseEdit = basewnd.toObject(stEdit,"CWndBase")
bseEdit:SetFocus(bseEdit)
regEvent(expwnd)
g_pConsole = expwnd


