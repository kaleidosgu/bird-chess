

LuaInt = {
			60,  --G_nMaxBirdCount
			10,  --G_nPerGroundCount
			15,  --G_nPerGroundCreate
			8888, --G_nPort
			}


luaBool = true

LuaStr ={
--	"172.18.10.50",  --IP �����
	"58.247.205.20",  --��������
	"test",
	"����",
	"E:\\test.lua"
}

function GetLuaStr(nIndex)
	return LuaStr[nIndex]
end

function GetLuaInt(nIndex)
	return LuaInt[nIndex]
end


print(GetLuaInt(3))
