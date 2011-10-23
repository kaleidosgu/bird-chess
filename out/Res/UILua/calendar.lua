local function OnMessage(self,nUIEvent,nDlgID)
	local keyres = WND_RESULT_NO
	if nUIEvent == 0 then
		if nDlgID >= self.btn_start then
			consoleAddText(nDlgID - self.btn_start)
		end
	end
	return keyres
end
function createTmpUI(strFile)
	local pMain = basewnd.toObject(g_UIGlobal["ptMainClient"],"CWndBase")
	local ptReturn = CreateUI(strFile,true,pMain,1,400)
	if ptReturn ~= nil then	
		ptReturn:SetScript( "OnMessage",OnMessage )
		local pWndBirdChat = CWndBase:new()
		ptReturn.btn_start = 100
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
		ptReturn.btnTable = {}
		for txtIdx = 1, 31 do 
			xpos = ( (txtIdx - 1) % 7 ) * xdiff
			local st1 = CWndStatic:new()
			strNumber = tonumber(txtIdx)
			st1:CreateNoFont(xpos,ypos,20,20,strNumber,pWndBirdChat,0)
			st1:SetScale(0.4)
			
			local btn = CWndButton:new()
			btn:Create(xpos,ypos,20,20,"",pWndBirdChat,ptReturn.btn_start + txtIdx)
			if txtIdx % 7 == 0 then
				ypos = ypos + ydiff
			end
			ptReturn.txtTable[txtIdx] = st1
			ptReturn.btnTable[txtIdx] = btn
		end
		
		consoleClear()
		-- local todayTimeTable = os.date('*t')
		-- local nCurYear = 	todayTimeTable["year"]
		-- local nCurMonth = 	todayTimeTable["month"]
		local nCurYear = 	2011
		local nCurMonth = 	2
		consoleAddText("Today is " .. nCurYear .. "-" .. nCurMonth)
		
		local tmpTable = {}
		
		tmpTable.year  =  nCurYear
		tmpTable.month =  nCurMonth
		tmpTable.day   =  1
		local firstDayCurMonth = os.time(tmpTable)
		tmpTable = os.date('*t',firstDayCurMonth)
		local firstWday = tmpTable.wday
		
		local days_in = get_days_in_month(nCurMonth,nCurYear)
		
		local pTmpTxt = nil
		local pTmpBtn = nil
		local nDaysIdx = 1
		
		local ypos = 0
		local xpos = 0
		consoleAddText(days_in)
		-- 42是因为一个月的日历会有6*7个格子
		for txtIdx = firstWday,42 do 
			pTmpTxt = ptReturn.txtTable[nDaysIdx]
			pTmpBtn = ptReturn.btnTable[nDaysIdx]
			
			xpos = ( (txtIdx - 1) % 7 ) * xdiff
			pTmpTxt:MoveWindow(xpos,ypos)
			pTmpBtn:MoveWindow(xpos,ypos)
			if txtIdx % 7 == 0 then
				ypos = ypos + ydiff
			end
			
			if nDaysIdx <= days_in then
				pTmpTxt:ShowWindow(true)
				pTmpBtn:ShowWindow(true)
			elseif nDaysIdx <= 31 then
				pTmpTxt:ShowWindow(false)
				pTmpBtn:ShowWindow(false)
				consoleAddText("bb")
			end
			if nDaysIdx >= 31 then
				break
			end
			
			nDaysIdx = nDaysIdx + 1
		end
	end
end