curBirdPt = nil;
function res( index )
return 	EnitySize[index].name,
	EnitySize[index].nIndex,
	EnitySize[index].x,
	EnitySize[index].y,
	EnitySize[index].width,
	EnitySize[index].height,
	curBirdPt
end

function GetPicSize(index)
	GetPicSizeFromLua(res(index))
end


EnitySize = {
{name="MonsterBase",nIndex = 0,x=902,y=798,width=48,height=46},
{name="MonsterBase",nIndex = 1,x=851,y=891,width=50,height=47},
{name="MonsterBase",nIndex = 2,x=904,y=889,width=50,height=46},
{name="MonsterBase",nIndex = 10,x=851,y=891,width=50,height=47},
{name="RushAngryBird",nIndex = 0,x=831,y=529,width=100,height=75,},
{name="RushAngryBird",nIndex = 1,x=953,y=607,width=95,height=75},
{name="RushAngryBird",nIndex = 2,x=930,y=525,width=100,height=75},
{name="RushAngryBird",nIndex = 10,x=831,y=529,width=100,height=75},
{name="StrongAngryBird",nIndex = 0,x=715,y=446,width=62,height=81},
{name="StrongAngryBird",nIndex = 1,x=665,y=678,width=66,height=73},
{name="StrongAngryBird",nIndex = 2,x=651,y=447,width=62,height=81},
{name="SmartAngryBird",nIndex = 0,x=730,y=800,width=60,height=53},
{name="SmartAngryBird",nIndex = 1,x=732,y=746,width=60,height=53},
{name="SmartAngryBird",nIndex = 2,x=862,y=745,width=70,height=52},
{name="SmartAngryBird",nIndex =10,x=790,y=745,width=62,height=53},
{name="EggBird",nIndex =0,x=665,y=820,width=49,height=58},
{name="CreateEggBird",nIndex =0,x=673,y=753,width=46,height=65},
{name="CreateEggBird",nIndex =1,x=673,y=753,width=46,height=65},
{name="CreateEggBird",nIndex =2,x=405,y=340,width=87,height=106},
{name="SmallBird",nIndex =0,x=823,y=254,width=88,height=97},
{name="SmallBird",nIndex =1,x=405,y=340,width=87,height=106},
{name="SmallBird",nIndex =2,x=0,y=537,width=32,height=32},
{name="AddLifeItem",nIndex =0,x=567,y=290,width=50,height=48},  --567,290,50,48,0
{name="SlowBirdItem",nIndex =0,x=513,y=290,width=51,height=45},       --513,290,51,45
{name="SlowBirdItem",nIndex =10,x=672,y=290,width=48,height=43},
{name="AddPlayerSpeedItem",nIndex =0,x=447,y=290,width=65,height=51},   --447,290,65,51,0);
{name="BecomeBirdItem",nIndex =0,x=726,y=289,width=51,height=46}, --		pEntity->SetTexture(tex,726,289,51,46,0);
--{name="QueenKissItem",nIndex =10,x=616,y=281,width=54,height=56},  --616,281,54,56,10);
{name="QueenKissItem",nIndex =0,x=772,y=253,width=48,height=54},  --772,253,48,54,0);
{name="RealPigItem",nIndex =0,x=497,y=450,width=76,height=38},
{name="HeroPig",nIndex =0 ,x=500,y=488,width=67,height=55},  -- //带黄帽子
--{name="HeroPig",nIndex =1,x=781,y=616,width=50,height=50},  -- //带鸟面具
{name="HeroPig",nIndex =1,x=507,y=876,width=52,height=49},
{name="HeroPig",nIndex =2,x=781,y=616,width=50,height=50},  -- //带鸟面具
{name="HeroPig",nIndex =3,x=592,y=680,width=49,height=46},
{name="HeroPig",nIndex =4,x=590,y=731,width=49,height=47},
{name="HeroPig",nIndex =5,x=587,y=779,width=53,height=49},
{name="HeroPig",nIndex =6,x=587,y=829,width=53,height=49},
{name="HeroPig",nIndex =7,x=501,y=818,width=53,height=50},
{name="HeroPig",nIndex =8,x=507,y=876,width=52,height=49},
{name="HeroPig",nIndex =10,x=587,y=779,width=53,height=49},--nIndex =10,x=507,y=876,width=52,height=49},
{name="HeroPig",nIndex =14,x=781,y=616,width=50,height=50},  -- //带鸟面具
{name="HeroPig",nIndex =15,x=500,y=488,width=67,height=55},  -- //带黄帽子
{name="HeroPig",nIndex =16,x=501,y=583,width=47,height=46},  -- //带黄帽子
{name="HeroPig",nIndex =20,x=781,y=616,width=50,height=50},

{name="SprMouse",nIndex     =0,x=16,y=474,width=20,height=29},  --16,474,20,29
{name="SprBirdDead",nIndex  =0,x=19,y=730,width=19,height=19},  --19,730,19,19
{name="SprBirdDead2",nIndex =0,x=24,y=627,width=12,height=26},  --24,627,12,26
{name="BGCloud2",nIndex =0,x=48,y=0,width=489,height=50},  --24,627,12,26
{name="BGCloud3",nIndex =0,x=48,y=51,width=489,height=65},  --24,627,12,26
{name="BGCloud4",nIndex =0,x=48,y=124,width=489,height=73},  --24,627,12,26
{name="BGCloud5",nIndex =0,x=66,y=197,width=358,height=78},  --24,627,12,26

{name="BulletNormal",nIndex =0,x=134,y=944,width=30 ,height=23},       --134,944, 30  ,23
{name="BulletDouble",nIndex =0,x= 92,y=944,width=30 ,height=23}, 	       -- 92,944,30 ,23
{name="BulletPower",nIndex  =0,x= 41,y=951,width=21 ,height=17},  	   -- 41,951,21 ,17
{name="BulletBomb1",nIndex  =0,x=165,y=276,width=132,height=124},       --164,276, 134, 124
{name="BulletBomb2",nIndex  =0,x=169,y=513,width=116,height=112},       --169,512, 116, 113
{name="BulletBomb3",nIndex  =0,x=878,y=0,width=128,height=124},       --169,512, 116, 113
{name="BulletBomb4",nIndex  =0,x=841,y=441,width=87,height=86},

{name="PlayerFace-1",nIndex =1,x=81,y=470,width=54,height=54},  -- //带鸟面具
{name="PlayerFace0",nIndex = 10,x=851,y=891,width=50,height=47},  -- //红
{name="PlayerFace1",nIndex = 10,x=831,y=529,width=100,height=75},  -- //绿色
{name="PlayerFace2",nIndex = 0,x=730,y=800,width=60,height=53},  -- //黄
{name="PlayerFace3",nIndex = 2,x=651,y=447,width=62,height=81},		--//黑
{name="PlayerFace4",nIndex =16,x=501,y=583,width=47,height=46},		--//猪
{name="PlayerFaceBG",nIndex  =0,x=692,y=708,width=112,height=117},  -- //带黄帽子

{name="BirdBackBack",nIndex  =0,x=155,y=258,width=37,height=39},
{name="BirdBack",nIndex  =0,x=27,y=259,width=38,height=36},
{name="BirdGo",nIndex  =0,x=69,y=259,width=38,height=36},
{name="BirdGoGo",nIndex  =0,x=114,y=258,width=37,height=39},

{name="CardBGReady",nIndex  =0,x=33,y =32,width=145,height=210},
{name="CardBGWaiting",nIndex  =0,x=722,y=20,width=145,height=210},
{name="WinPrincess",nIndex  =0,x=616,y=281,width=54,height=56},


}

function GetBirdPicCount()
	return table.getn(EnitySize)
end

--LuaInt = {GetBirdPicCount(),2}

LuaInt = {GetBirdPicCount(),2}

luaBool = true

LuaStr ={"test","测试","E:\\test.lua"}

function GetLuaStr(nIndex)
	return LuaStr[nIndex]
end

function GetLuaInt(nIndex)
	return LuaInt[nIndex]
end


print(GetLuaInt(1))
