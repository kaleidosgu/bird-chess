#include "RealPigItem.h"
#include "GameDefine.h"

RealPigItem::RealPigItem(HeroPig& rMainPlayer):m_rMainPlayer(rMainPlayer)
{
	SetEffectTime(12);
}

RealPigItem::~RealPigItem(void)
{
}


void RealPigItem::ItemEffect()
{
	PlaySound(36);
	m_rMainPlayer.SetCurrentSprID(15);
	m_nChangeRoleSpr++;
	m_rMainPlayer.SetPlayerStat(ENUM_PLAY_UNCONQUERABLE);
	ItemBase::ItemEffect();
}


void RealPigItem::RemoveItemEffect()
{
	m_nChangeRoleSpr--;
	if(m_nChangeRoleSpr == 0)
	{
		m_rMainPlayer.SetCurrentSprID(m_rMainPlayer.GetLife());
		m_rMainPlayer.SetPlayerStat(ENUM_PLAY_NORMAL);
	}
}


void RealPigItem::BeforRemoveItemEffect(float fdt)
{
	static float temptime(0.0);
	temptime +=fdt;

	if(temptime>1)
	{
		temptime=0.0;
	}
	else if(temptime>0.5)
	{
		if(m_nChangeRoleSpr == 1)
			m_rMainPlayer.SetCurrentSprID(15);
	}
	else
	{
		if(m_nChangeRoleSpr == 1)
			m_rMainPlayer.SetCurrentSprID(m_rMainPlayer.GetLife());
	}
}
