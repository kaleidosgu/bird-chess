function CreateLuaWnd(parent,ptx,pty)
	local pWndBse = CUIExportWndBase:new()
	pWndBse:Create(ptx,pty,0,0,parent,0);
	return pWndBse
end

function CreateUI(strRes,bDestroy,pParent,ptx,pty)
	local ptReturn = nil
	local bCreate = false
	if g_tUIMap[strRes] == nil then
		bCreate = true
		local strRes = string.format("%s%s",g_UIcurDir , strRes)
	else
		if bDestroy == true then
			local ptDestroy = g_tUIMap[strRes]
			if ptDestroy ~= nil then
				local ptDestroyBase = basewnd.toObject(ptDestroy,"CWndBase")
				ptDestroyBase:Destroy()
			else
				consoleAddText(strRes .. " is nil, can't destroy")
			end
			bCreate = true
			consoleAddText(strRes .. " destroy!")
		else
			consoleAddText(strRes .. " UI exist, can't create!")
		end
	end
	if bCreate == true then
		ptReturn = CreateLuaWnd(pParent,ptx,pty)
		g_tUIMap[strRes] = ptReturn
	end
	return ptReturn
end

function IsFileExist( strFile )
	local tmpHandle = io.open(strFile,"r")
	local bExist = false
	if tmpHandle ~= nil then
		bExist = true
		tmpHandle:close()
	end
	return bExist
end