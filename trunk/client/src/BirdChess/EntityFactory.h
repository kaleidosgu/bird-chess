#pragma once
#pragma warning( disable : 4244 )
#pragma warning( disable : 4018 )
#pragma warning( disable : 4305 )

#include "MonsterBase.h"
#include "ItemBase.h"
#include "BulletBase.h"
#include "HeroPig.h"
#include "hge.h"
#include <list>
#include <map>

using namespace std;

class CLuaParse;
struct EntitySize;
class EntityFactory
{
public:
	EntityFactory(void);
	~EntityFactory(void);
	static EntityFactory& Instance();

	void SetHTEXTURE(HTEXTURE tex);
	void Init();
	hgeSprite* CreateSprite(char* name);

	MonsterBase* CreateBird(int nId,list<MonsterBase*>& G_AngryBird,EntityBase*MainPlayer = NULL);
	ItemBase* CreateItem(int nId,list<BulletBase*>& G_Bulletlist ,list<MonsterBase*>& G_AngryBird ,HeroPig*MainPlayer = NULL);
	HeroPig* CreatePlayer();
	BulletBase* CreateBulletByName(char* name,bool IsBomb = false);
private:
	CLuaParse* m_pLua;
	HTEXTURE m_pTex;
	map<string,list<EntitySize*>> m_mapEntitySize;
	
};
