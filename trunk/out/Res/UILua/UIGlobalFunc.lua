function CreateLuaWnd(parent,ptx,pty)
	local pWndBse = CUIExportWndBase:new()
	pWndBse:Create(ptx,pty,0,0,parent,0);
	return pWndBse
end

function CreateUI(strRes,bDestroy,pParent,ptx,pty,strsimple)
	local ptReturn = nil
	local bCreate = false
	if g_tUIMap[strsimple] == nil then
		bCreate = true
		local strRes = string.format("%s%s",g_UIcurDir , strRes)
	else
		if bDestroy == true then
			local ptDestroy = g_tUIMap[strsimple]
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
		ptReturn:SetPath(strRes)
		g_tUIMap[strsimple] = ptReturn
		consoleAddText("haha" .. strRes)
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

function get_days_in_month(month, year)
	local days_in_month = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
	local d = days_in_month[month]

	-- check for leap year
	if (month == 2) then
		if (math.mod(year,4) == 0) then
			if (math.mod(year,100) == 0)then
				if (math.mod(year,400) == 0) then
					d = 29
				end
			else
				d = 29
			end
		end
	end

	return d
end
function isUIExist( str )
	local substr = g_UIcurDir .."\\res\\uilua\\" .. str .. ".lua"
	local res = false
	if g_tUIMap[substr] ~= nil then
		res  =true
	end
	--consoleAddText(tostring(substr))
	return res
end
function getUIPoint( str )
	local substr = string.format("\/res\/uilua\/%s.lua",str)
	substr = g_UIcurDir .. substr
	consoleAddText("haha" .. substr)
	return g_tUIMap[str],substr
end
function hideLoginWnd( )
	local pWndLogin,substr = getUIPoint( "login" )
	if pWndLogin ~= nil then
		pWndLogin:ShowWindow(false)
	else
		return substr
	end
end
