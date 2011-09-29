#pragma once
#include "itembase.h"
#include "EntityBase.h"

class AddLifeItem :
	public ItemBase
{
public:
	AddLifeItem(EntityBase& rMainPlayer);
	virtual ~AddLifeItem(void);
	virtual void ItemEffect();
private:
	EntityBase& m_rMainPlayer;

};
