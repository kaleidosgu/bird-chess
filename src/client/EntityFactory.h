#pragma once
#pragma warning( disable : 4244 )
#pragma warning( disable : 4018 )
#pragma warning( disable : 4305 )

//#include "MonsterBase.h"
//#include "ItemBase.h"
//#include "BulletBase.h"
//#include "HeroPig.h"
#include "hge.h"
#include <list>
#include <map>

using namespace std;

class CLuaParse;
struct EntitySize;
class CBirdEntity;
class CCloudEntity;
class hgeSprite;
class EntityFactory
{
public:
	EntityFactory(void);
	~EntityFactory(void);
	static EntityFactory& Instance();

	void SetHTEXTURE(HTEXTURE pTex,HTEXTURE pTexUI);

	HTEXTURE GetUITEXTURE() {return m_pTexCardUI;};
	HTEXTURE GetEntityTexture();
	void Init();
	hgeSprite* CreateSprite(char* name);
	hgeSprite* CreateUISprite(char* name);

	//MonsterBase* CreateBird(int nId,list<MonsterBase*>& G_AngryBird,EntityBase*MainPlayer = NULL);
	//ItemBase* CreateItem(int nId,list<BulletBase*>& G_Bulletlist ,list<MonsterBase*>& G_AngryBird ,HeroPig*MainPlayer = NULL);
	//HeroPig* CreatePlayer();
	//BulletBase* CreateBulletByName(char* name,bool IsBomb = false);

	CBirdEntity* CreateUIBird(int nType);
	CCloudEntity* CreateUICloud(int nIndex);
	bool GetEntityList( std::list < EntitySize* >& rLst, const std::string& rKey );
private:
	CLuaParse* m_pLua;
	HTEXTURE m_pTex;
	HTEXTURE m_pTexCardUI;
	map<string,list<EntitySize*>> m_mapEntitySize;
	
};
