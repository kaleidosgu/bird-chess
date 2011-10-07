UITest = {m_nSpeed = 30,m_bShowEdit = true,movewnd = nil,context = nil,myedit = nil,mttime = 0,
}
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
		consoleAddText("d")
	elseif eKey == HGE_KEY_ENTER then
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
			CreateUI(substr)
		end
		keyres = WND_RESULT_YES
	end
	return keyres
end

local function regEvent( parent )
	parent:SetScript( "OnUpdate",OnUpdate )
	parent:SetScript( "OnKeyDown",OnKeyDown )
end

function consoleAddText( str )
	g_pConsole.context:AddText(str)
	g_pConsole.context:SetMask("*")
end


local pDesk = basewnd.toObject(g_UIGlobal["ptDesk"],"CWndBase")
local expwnd = CreateLuaWnd(pDesk)
expwnd.m_bShowEdit = true
expwnd.m_nSpeed = 30
local bseexp = basewnd.toObject(expwnd,"CWndBase")

local movewnd = CWndBase:new()
expwnd.movewnd = movewnd
movewnd:Create(0,0,0,0,bseexp,0)

local bg = CWndLoadPicture:new()
bg:Create( 0, 0, "res/UILua/ConsoleBG.png", movewnd, 0 );

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
stEdit:CreateNoFont(0,280,100,28,movewnd,0)
stEdit:SetNotifyParent(true)
stEdit:SetText("")
expwnd.myedit = stEdit
local bseEdit = basewnd.toObject(stEdit,"CWndBase")
bseEdit:SetFocus(bseEdit)
regEvent(expwnd)
g_pConsole = expwnd


