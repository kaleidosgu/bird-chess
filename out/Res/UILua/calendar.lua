function createTmpUI(strFile)
	local pMain = basewnd.toObject(g_UIGlobal["ptMainClient"],"CWndBase")
	local ptReturn = CreateUI(strFile,true,pMain,1,400)
	if ptReturn ~= nil then		
		local pWndBirdChat = CWndBase:new()
		pWndBirdChat:Create(0,0,0,0,ptReturn,0)
		pWndBirdChat:ShowWindow(true)
		
		local sbg = CWndLoadPicture:new()
		sbg:Create( 0, 0, "res/uilua/calendarbg.PNG", pWndBirdChat, 0, false, 0,0,0,0 )
		
		local xdiff = 20
		local ydiff = 20
		local ypos = 0
		local xpos = 0
		local strNumber = ""
		ptReturn.txtTable = {}
		for txtIdx = 1, 31 do 
			xpos = ( (txtIdx - 1) % 7 ) * xdiff
			local st1 = CWndStatic:new()
			strNumber = tonumber(txtIdx)
			st1:CreateNoFont(xpos,ypos,20,20,strNumber,pWndBirdChat,0)
			st1:SetScale(0.4)
			if txtIdx % 7 == 0 then
				ypos = ypos + ydiff
			end
			ptReturn.txtTable[txtIdx] = st1
		end
		
		consoleClear()
		local todayTimeTable = os.date('*t')
		local nCurYear = 	todayTimeTable["year"]
		local nCurMonth = 	todayTimeTable["month"]
		-- local nCurYear = 	2011
		-- local nCurMonth = 	9
		consoleAddText("Today is " .. nCurYear .. "-" .. nCurMonth)
		
		local tmpTable = {}
		
		tmpTable.year  =  nCurYear
		tmpTable.month =  nCurMonth
		tmpTable.day   =  1
		local firstDayCurMonth = os.time(tmpTable)
		tmpTable = os.date('*t',firstDayCurMonth)
		local firstWday = tmpTable.wday
		consoleAddText(firstWday)
		
		local days_in = get_days_in_month(nCurMonth,nCurYear)
		
		local pTmpTxt = nil
		local nDaysIdx = 1
		
		local ypos = 0
		local xpos = 0
		consoleAddText(days_in)
		-- 42是因为一个月的日历会有6*7个格子
		for txtIdx = firstWday,42 do 
			pTmpTxt = ptReturn.txtTable[nDaysIdx]
			if pTmpTxt == nil then
				consoleAddText("nil index is = " ..nDaysIdx)
			end
			
			xpos = ( (txtIdx - 1) % 7 ) * xdiff
			pTmpTxt:MoveWindow(xpos,ypos)
			if txtIdx % 7 == 0 then
				ypos = ypos + ydiff
			end
			
			if nDaysIdx <= days_in then
				pTmpTxt:ShowWindow(true)
			elseif nDaysIdx < 31 then
				pTmpTxt:ShowWindow(false)
			end
			if nDaysIdx >= 31 then
				break
			end
			
			nDaysIdx = nDaysIdx + 1
		end
	end
end