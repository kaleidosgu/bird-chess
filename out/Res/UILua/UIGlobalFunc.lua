function CreateLuaWnd(parent)
	local pWndBse = CUIExportWndBase:new()
	pWndBse:Create(0,0,0,0,parent,0);
	return pWndBse
end

