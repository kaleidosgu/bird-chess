#pragma once
#include "itembase.h"
#include "HeroPig.h"

class QueenKissItem :
	public ItemBase
{
public:
	QueenKissItem(HeroPig& rMainPlayer);
	virtual ~QueenKissItem(void);
	virtual  void ItemEffect();
	virtual  void RemoveItemEffect();
	virtual  void BeforRemoveItemEffect(float fdt);

private:
	HeroPig& m_rMainPlayer;
};
