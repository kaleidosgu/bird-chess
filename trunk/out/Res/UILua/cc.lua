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