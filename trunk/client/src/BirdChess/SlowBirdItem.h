#pragma once
#include "itembase.h"
#include "BulletBase.h"
#include "MonsterBase.h"
#include <list>

class SlowBirdItem :
	public ItemBase
{
public:
	SlowBirdItem(std::list<BulletBase*>& rBulletlist,std::list<MonsterBase*>& rAngryBird);
	virtual ~SlowBirdItem(void);
	virtual void ItemEffect();
	virtual void RemoveItemEffect();
	virtual void BeforRemoveItemEffect(float ftd);
private:
	std::list<BulletBase*>& m_rBulletlist;
	std::list<MonsterBase*>& m_rAngryBird;
	static int m_nChangeBG;
};
