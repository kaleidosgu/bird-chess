local function OnKeyDown(self,eKey)
	
end
local function OnUpdate(self,e)
end

local function regEvent( parent )
	parent:SetScript( "OnKeyDown",OnKeyDown )
	parent:SetScript( "OnUpdate",OnUpdate )
end

function createTmpUI(strFile)
	local pMain = basewnd.toObject(g_UIGlobal["ptMainClient"],"CWndBase")
	local ptReturn = CreateUI(strFile,true,pMain,450,250)
	if ptReturn ~= nil then
		local ptReturnbse = basewnd.toObject(ptReturn,"CWndBase")		
		local pPicture = CWndLoadPicture:new();
		pPicture:Create(0,0,"res/pic/EntityPic.png",ptReturnbse,0,true,0,378,33,34);
		pPicture:SetFlip(true,false,false);
		regEvent(ptReturn)
	end
end