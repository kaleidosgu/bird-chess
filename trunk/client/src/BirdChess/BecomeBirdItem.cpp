#include "BecomeBirdItem.h"
#include "GameDefine.h"

BecomeBirdItem::BecomeBirdItem(HeroPig& rMainPlayer):m_rMainPlayer(rMainPlayer)
{
	SetEffectTime(12);
}

BecomeBirdItem::~BecomeBirdItem(void)
{
}

void BecomeBirdItem::ItemEffect()
{
	PlaySound(37);
	m_rMainPlayer.SetCurrentSprID(14);
	m_rMainPlayer.SetPlayerStat(ENUM_PLAY_VISIBLE);
	m_rMainPlayer.SetChangeBulletRation(3.0);
	m_nChangeRoleSpr++;
	ItemBase::ItemEffect();
}


void BecomeBirdItem::RemoveItemEffect()
{
	m_nChangeRoleSpr--;
	if(m_nChangeRoleSpr == 0)
	{
		m_rMainPlayer.SetCurrentSprID(m_rMainPlayer.GetLife());
		m_rMainPlayer.SetChangeBulletRation(0.0);
		m_rMainPlayer.SetPlayerStat(ENUM_PLAY_NORMAL);
	}
}

void BecomeBirdItem::BeforRemoveItemEffect(float fdt)
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
			m_rMainPlayer.SetCurrentSprID(m_rMainPlayer.GetLife());
	}
	else
	{
		if(m_nChangeRoleSpr == 1)
			m_rMainPlayer.SetCurrentSprID(14);
	}
}