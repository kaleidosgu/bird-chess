local function OnKeyDown(self,eKey)
	local keyres = WND_RESULT_NO
	-- consoleAddText(eKey)
	if eKey == 39 then
		self.sbg:MoveWindow(self.sbg:GetClientPos().x + 5,self.sbg:GetClientPos().y)
	elseif eKey == 37 then
		self.sbg:MoveWindow(self.sbg:GetClientPos().x - 5,self.sbg:GetClientPos().y)
	end
	--consoleClear( )
	if self.faketable ~= nil then
		for i,v in ipairs(self.faketable ) do
			local xpos = self.sbg:GetClientPos().x +  (4 - i) * 80
			if v.startpos > xpos then
				v.fakeUpdate = true
				v:ShowWindow(true)
			end
		end
	else
	end

	return keyres
end
local function OnUpdate(self,e)
	if self.tickcounts > 0.03 then
		if self.faketable == nil then
		else
			for i,v in ipairs(self.faketable ) do
				if v.fakeUpdate == true then
					if v.startpos <= self.sbg:GetClientPos().x  then
						v.fakeUpdate = false
						v:ShowWindow(false)
					end
					local ndiff = -5
					v:MoveWindow(v:GetClientPos().x + ndiff, v:GetClientPos().y)
					v.startpos = v:GetClientPos().x
				end
			end
		end

		self.tickcounts = self.tickcounts - 0.03
	end
	self.tickcounts = self.tickcounts + e
end

local function regEvent( parent )
	parent:SetScript( "OnKeyDown",OnKeyDown )
	parent:SetScript( "OnUpdate",OnUpdate )
	parent:SetScript( "OnMessage",OnMessage )
end
local function CreateFake( parent , a, r, g, b,x,y,startposX )
	local fake = CWndLoadPicture:new()
	fake:Create( x, y, "res/uilua/akuma.PNG", parent, 0, false, 0,0,0,0 )
	fake:ShowWindow(false)
	fake:SetBlendMode(3)
	fake:SetColor(GetARGB(a,r,g,b))
	fake.fakeUpdate = false
	fake.startpos = startposX
	return fake
end
function createTmpUI(strFile)
	local pMain = basewnd.toObject(g_UIGlobal["ptMainClient"],"CWndBase")
	local ptReturn = CreateUI(strFile,true,pMain,1,0)
	if ptReturn ~= nil then

		--consoleClear( )
		ptReturn.faketable = {}
		local sPos = 850
		local yPos = 500
		for i = 1, 3 do
			local fake = CreateFake( ptReturn, i * 85,255,255,255,sPos,yPos,sPos)
			if fake == nil then
			else
				table.insert(ptReturn.faketable,fake)
			end
		end
		local sbg = CWndLoadPicture:new()
		sbg:Create( sPos, yPos, "res/uilua/akuma.PNG", ptReturn, 0, false, 0,0,0,0 )
		ptReturn.sbg = sbg

		local xpos = sbg:GetClientPos().x

		ptReturn:SetFocus(nil)
		ptReturn.tickcounts = 0
		regEvent( ptReturn )
		isUIExist("akuma")

	end
end
