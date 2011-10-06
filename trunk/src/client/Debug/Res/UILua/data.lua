UITest = {balance = 0}

function createwnd( parent )
local bseDesk = basewnd.toBase(parent)
local expwnd = CUIExportWndBase:new()
local bseexp = basewnd.toBase(expwnd)

expwnd:Create( 0,0,100,100, bseDesk, 0 );

UITest:func1(expwnd)


local pic = CWndPicture:new()
pic:Create( 3,3,"StrongAngryBird", bseexp, 3 );
UITest.kk = pic

return 100
end



function UITest:func1( parent )
	local f = self
	f.mttime = 0
	parent:SetScript( function(e)
		self.mttime = self.mttime + e
		 local bbwnd = basewnd.toBase(UITest.kk)
		 bbwnd:MoveWindow(self.mttime * 1000,3,true)
		if self.mttime > 3 then
			--self.mttime = 0;
		end
		return self.mttime
	end)
	return 3	
end


