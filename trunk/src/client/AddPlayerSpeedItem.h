#pragma once
#include "itembase.h"
#include "HeroPig.h"
#include "hgeparticle.h"

class AddPlayerSpeedItem :
	public ItemBase
{
public:
	AddPlayerSpeedItem(HeroPig& rMainPlayer);
	virtual ~AddPlayerSpeedItem(void);
	virtual void ItemEffect();
	virtual void RemoveItemEffect();
	virtual void Update(float dt);
	virtual void Render();
private:
	HeroPig& m_rMainPlayer;
	hgeParticleSystem* MainPar;
};
