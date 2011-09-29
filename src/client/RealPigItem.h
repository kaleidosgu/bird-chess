#pragma once
#include "itembase.h"
#include "HeroPig.h"

class RealPigItem :
	public ItemBase
{
public:
	RealPigItem(HeroPig& rMainPlayer);
	virtual ~RealPigItem(void);
	virtual  void ItemEffect();
	virtual  void RemoveItemEffect();
	virtual  void BeforRemoveItemEffect(float fdt);

private:
	HeroPig& m_rMainPlayer;
};
