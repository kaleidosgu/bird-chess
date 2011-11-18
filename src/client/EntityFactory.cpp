#include "EntityFactory.h"
#include "lua/CLuaParse.h"

#include "hgeparticle.h"
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