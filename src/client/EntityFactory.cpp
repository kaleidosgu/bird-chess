#include "EntityFactory.h"
#include "StrongAngryBird.h"
#include "SmartAngryBird.h"
#include "RushAngryBird.h"
#include "SlowBirdItem.h"
#include "AddLifeItem.h"
#include "AddPlayerSpeedItem.h"
#include "BecomeBirdItem.h"
#include "RealPigItem.h"
#include "QueenKissItem.h"
#include "BombBullet.h"
#include "EggBird.h"
#include "CreateEggBird.h"
#include "SmallBird.h"
#include "ActHeroPig.h"

#include "lua/CLuaParse.h"

#include "GameDefine.h"
#include "UIBirdEntity.h"
#include "UICloudEntity.h"
#include <assert.h>

EntityFactory::EntityFactory(void):m_pTexCardUI(NULL)
{
	m_pLua = new CLuaParse();
	m_pLua->initLua();
	int  nRes = m_pLua->LoadLuaFile("Res\\config\\EntityConfig.lua");
	assert(nRes==0);
	Init();
}

void EntityFactory::Init()
{
	int i=1;							//Lua的索引是从1开始
	int nflag = 0;
	if(m_pLua->GetLuaIntByIndex("GetLuaInt",1,nflag))
	{
		for(;i<=nflag;i++)
		{
			EntitySize*pBS = new EntitySize();
			m_pLua->DoLuaFile("GetPicSize",i,pBS);
			m_mapEntitySize[pBS->sName].push_back(pBS);
		}
	}	
}
EntityFactory::~EntityFactory(void)
{
}



BulletBase* EntityFactory::CreateBulletByName(char* name,bool IsBomb)
{
	BulletBase* pBasebullet = NULL;
	if(IsBomb)
	{
		pBasebullet = new BombBullet();
		pBasebullet->SetTexture(m_pTex,m_mapEntitySize[name]); 
	}
	else
	{
		pBasebullet = new BulletBase();
		pBasebullet->SetTexture(m_pTex,m_mapEntitySize[name]); 
	}
	return pBasebullet;
}

hgeSprite* EntityFactory::CreateSprite(char* name)
{

	EntitySize* pES = *(m_mapEntitySize[name].begin());
	return new hgeSprite(m_pTex,pES->x,pES->y,pES->w,pES->h);
}

hgeSprite* EntityFactory::CreateUISprite(char* name)
{

	EntitySize* pES = *(m_mapEntitySize[name].begin());
	return new hgeSprite(m_pTexCardUI,pES->x,pES->y,pES->w,pES->h);
}


void EntityFactory::SetHTEXTURE(HTEXTURE pTex,HTEXTURE pTexUI)
{
	m_pTex = pTex;
	m_pTexCardUI = pTexUI;
}

EntityFactory& EntityFactory::Instance()
{
	static EntityFactory Factory;
	return Factory;
}

MonsterBase* EntityFactory::CreateBird(int nId,list<MonsterBase*>& G_AngryBird,EntityBase *MainPlayer)
{

	MonsterBase*pEntity = NULL;
	int nLocatX = 0;
	int nLocatY = 0;
	int nRand = rand();
	if(nRand%2 )
	{
		nLocatX = nRand%SCREEN_WIDTH;
		if(rand()%2)
		{
			nLocatY = SCREEN_HEIGHT;
		}
	}
	else
	{
		nLocatY = nRand%SCREEN_HEIGHT;
		if(rand()%2)
		{
			nLocatX = SCREEN_WIDTH;
		}
	}
	switch (nId)
	{
		case ENUM_BIRDTYPE_REDANGRYBIRD:
			{
				pEntity = new MonsterBase();
				pEntity->SetTexture(m_pTex,m_mapEntitySize["MonsterBase"]); 
			}
			break;
		case ENUM_BIRDTYPE_RUSNANGRYBIRD:
			{
				pEntity = new RushAngryBird(*MainPlayer);
				pEntity->SetTexture(m_pTex,m_mapEntitySize["RushAngryBird"]); 
				nLocatY = nRand%SCREEN_HEIGHT;
				if(rand()%2)
					nLocatX = SCREEN_WIDTH-RUSHBIRDS_MOUSESIZE;
				else
					nLocatX = RUSHBIRDS_MOUSESIZE;
			}
			break;
		case ENUM_BIRDTYPE_STRONGANGRYBIRD:
			{
				pEntity = new StrongAngryBird();
				pEntity->SetTexture(m_pTex,m_mapEntitySize["StrongAngryBird"]); 
			}
			break;
		case ENUM_BIRDTYPE_SMARTANGRYBIRD:
			{

				pEntity = new SmartAngryBird(*MainPlayer);
				pEntity->SetTexture(m_pTex,m_mapEntitySize["SmartAngryBird"]); 
			}
			break;
		case ENUM_BIRDTYPE_EGGBIRD:
			{
				pEntity = new EggBird(G_AngryBird);
				pEntity->SetTexture(m_pTex,m_mapEntitySize["EggBird"]); 
			}
			break;	
		case ENUM_BIRDTYPE_CREATEEGGBIRD:
			{
				pEntity = new CreateEggBird(G_AngryBird);
				pEntity->SetTexture(m_pTex,m_mapEntitySize["CreateEggBird"]); 
				nLocatX = nRand%SCREEN_WIDTH;
				if(rand()%2)
					nLocatY = SCREEN_HEIGHT-pEntity->GetWidth();
				else
					nLocatY = pEntity->GetWidth();
			}
			break;
		case ENUM_BIRDTYPE_SMALLBIRD:
			{
				pEntity = new SmallBird(G_AngryBird,*MainPlayer);
				pEntity->SetTexture(m_pTex,m_mapEntitySize["SmallBird"]); 
			}
			break;
		default:
			{
				pEntity = new MonsterBase();
				pEntity->SetTexture(m_pTex,m_mapEntitySize["MonsterBase"]); 
			}
			break;
	}
	pEntity->SetLocation(nLocatX,nLocatY);
	return pEntity;
}


ItemBase* EntityFactory::CreateItem(int nId,list<BulletBase*>& G_Bulletlist,list<MonsterBase*>& G_AngryBird,HeroPig*MainPlayer)
{
	ItemBase*	pEntity = NULL;
	int nLocatX = rand()%SCREEN_WIDTH;
	int nLocatY = rand()%SCREEN_HEIGHT;
	switch (nId)
	{
	case ENUM_ITEMTYPE_ADDLIFE:
		{
			pEntity = new AddLifeItem(*MainPlayer);
			pEntity->SetTexture(m_pTex,m_mapEntitySize["AddLifeItem"]); 
		}
		break;
	case ENUM_ITEMTYPE_STOPALLBIRDS:
		{
			 pEntity = new SlowBirdItem(G_Bulletlist,G_AngryBird);
			pEntity->SetTexture(m_pTex,m_mapEntitySize["SlowBirdItem"]); 
		}
		break;
	case ENUM_ITEMTYPE_ADDSPEED:
		{
			pEntity = new AddPlayerSpeedItem(*MainPlayer);
			pEntity->SetTexture(m_pTex,m_mapEntitySize["AddPlayerSpeedItem"]); 
		}
		break;
	case ENUM_ITEMTYPE_BECOMEBIRD:
		{
			//AddLifeItem
			pEntity = new BecomeBirdItem(*MainPlayer);
			pEntity->SetTexture(m_pTex,m_mapEntitySize["BecomeBirdItem"]); 
			pEntity->GetCurrentSpr()->SetBlendMode(BLEND_COLORMUL | BLEND_ALPHABLEND | BLEND_NOZWRITE);
		}
		break;
	case ENUM_ITEMTYPE_REALPIG:
		{
			//AddLifeItem
			pEntity = new RealPigItem(*MainPlayer);
			pEntity->SetTexture(m_pTex,m_mapEntitySize["RealPigItem"]); 
		}
		break;
	case ENUM_ITEMTYPE_QUEENKISS:
		{
			//AddLifeItem
			pEntity = new QueenKissItem(*MainPlayer);
			pEntity->SetTexture(m_pTex,m_mapEntitySize["QueenKissItem"]); 
		}
		break;
	default:
		break;
	}
	pEntity->SetLocation(nLocatX,nLocatY);
	return pEntity;
}



HeroPig* EntityFactory::CreatePlayer()
{
	HeroPig* MainPlayer = new ActHeroPig();
	MainPlayer->SetTexture(m_pTex,m_mapEntitySize["HeroPig"]);
	return MainPlayer;
}


CBirdEntity* EntityFactory::CreateUIBird(int nType)
{
	CBirdEntity* pEntity = new CBirdEntity(nType);
	switch (nType)
	{
	case 0:
		pEntity->SetTexture(m_pTex,m_mapEntitySize["MonsterBase"]); 
		break;
	case 1:
		pEntity->SetTexture(m_pTex,m_mapEntitySize["RushAngryBird"]); 
		break;
	case 2:
		pEntity->SetTexture(m_pTex,m_mapEntitySize["SmartAngryBird"]); 
		break;
	case 3:
		pEntity->SetTexture(m_pTex,m_mapEntitySize["StrongAngryBird"]); 
		break;
	case 4:
		pEntity->SetTexture(m_pTex,m_mapEntitySize["HeroPig"]); 
		break;
	}
	
	return pEntity;
}

CCloudEntity* EntityFactory::CreateUICloud(int nIndex)
{
	CCloudEntity* pEntity = new CCloudEntity(nIndex);
	pEntity->SetTexture(m_pTex,m_mapEntitySize["BGCloud5"]); 
	return pEntity;
}

bool EntityFactory::GetEntityList( std::list < EntitySize* >& rLst, const std::string& rKey )
{
	bool bRes = false;
	map < string, list < EntitySize* > > ::iterator it = m_mapEntitySize.find( rKey );
	if( it != m_mapEntitySize.end() )
	{
		bRes = true;
		rLst = m_mapEntitySize[rKey];
	}
	return bRes;
}

HTEXTURE EntityFactory::GetEntityTexture()
{
	 return m_pTex;
}