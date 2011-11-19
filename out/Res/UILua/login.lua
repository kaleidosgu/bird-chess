local function OnKeyDown(self,eKey)
	local keyres = WND_RESULT_NO
	local strName 		= self.stEditName:GetText()
	local strPassword 	= self.stEditPassword:GetText()
	local nName 		= string.len(strName)
	local nPassword 	= string.len(strPassword)
	if nName > 0 and nPassword > 0 then
		consoleAddText("something")
		Login2Server(strName, strPassword)
		keyres = WND_RESULT_YES
	end
	return keyres
end

local function regEvent( parent )
	parent:SetScript( "OnKeyDown",OnKeyDown )
end

function createTmpUI(strFile,strSimple)
	local pMain = basewnd.toObject(g_UIGlobal["ptMainClient"],"CWndBase")
	local ptReturn = CreateUI(strFile,true,pMain,450,250,strSimple)
	if ptReturn ~= nil then
		local ptReturnbse = basewnd.toObject(ptReturn,"CWndBase")

		local pPicture = CWndLoadPicture:new();
		pPicture:Create(0,0,"res/pic/UILoginBG.png",ptReturnbse,0,true,460,6,231,232);

		local stEdit1 = CWndEdit:new();
		stEdit1:CreateNoFont(15,192,90,26,ptReturnbse,0)
		stEdit1:SetNotifyParent(true)
		stEdit1:SetText("")
		local ptEdit1Bse = basewnd.toObject(stEdit1,"CWndBase")
		ptEdit1Bse.m_bDrawBox = true

		local stEdit2 = CWndEdit:new();
		stEdit2:CreateNoFont(120,192,90,26,ptReturnbse,0)
		stEdit2:SetNotifyParent(true)
		stEdit2:SetText("")
		local ptEdit2Bse = basewnd.toObject(stEdit2,"CWndBase")
		ptEdit2Bse.m_bDrawBox = true
		ptReturn.stEditName 	= stEdit1
		ptReturn.stEditPassword = stEdit2
		regEvent(ptReturn)

		ptEdit1Bse:SetTabWnd(ptEdit2Bse)
		ptEdit2Bse:SetTabWnd(ptEdit1Bse)
		--ptEdit1Bse:SetFocus(ptEdit1Bse);
	end
end
