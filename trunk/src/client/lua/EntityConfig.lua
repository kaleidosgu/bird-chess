function add(index )
	TestInCPP(index )
end


BirdSize = {
{x=35,y=22,width=55,height=33},
{x=44,y=66,width=99,height=99},
}

curBirdPt = nil;
function res( index )
return BirdSize[index].x,
	BirdSize[index].y,
	BirdSize[index].width,
	BirdSize[index].height,
	curBirdPt
end

function GetPicSize(index)
	GetPicSizeFromLua(res(index))
end
	
luaBool = true

LuaStr ={"test","≤‚ ‘","E:\\test.lua"} 

function GetLuaStr(nIndex)
	return LuaStr[nIndex]
end

