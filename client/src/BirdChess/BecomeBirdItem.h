#pragma once
#include "itembase.h"
#include "HeroPig.h"

class BecomeBirdItem :
	public ItemBase
{
public:
	BecomeBirdItem(HeroPig& rMainPlayer);
	virtual ~BecomeBirdItem(void);
	virtual  void ItemEffect();
	virtual void RemoveItemEffect();
	virtual  void BeforRemoveItemEffect(float fdt);

private:
	HeroPig& m_rMainPlayer;
};
