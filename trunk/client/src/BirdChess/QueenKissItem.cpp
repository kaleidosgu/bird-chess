#include "QueenKissItem.h"
#include "GameDefine.h"

QueenKissItem::QueenKissItem(HeroPig& rMainPlayer):m_rMainPlayer(rMainPlayer)
{
	SetEffectTime(12);
	m_fDesotryTime = 0.5f;
}

QueenKissItem::~QueenKissItem(void)
{

}


void QueenKissItem::ItemEffect()
{
	PlaySound(34);
	m_rMainPlayer.SetCurrentSprID(16);
	m_rMainPlayer.SetLife(m_rMainPlayer.GetLife()+1);
	m_rMainPlayer.SetBulletLife(2);
	m_nChangeRoleSpr++;
	ItemBase::ItemEffect();
}


void QueenKissItem::RemoveItemEffect()
{
	m_nChangeRoleSpr--;
	if(m_nChangeRoleSpr == 0)
		m_rMainPlayer.SetCurrentSprID(m_rMainPlayer.GetLife());
	//m_rMainPlayer.SetShootCD(0.2f);
	m_rMainPlayer.SetBulletLife(1);
	//m_rMainPlayer.SetPlayerStat(ENUM_PLAY_NORMAL);
}


void QueenKissItem::BeforRemoveItemEffect(float fdt)
{
	//static float temptime(0.0);
	//temptime +=fdt;

	//if(temptime>1)
	//{
	//	temptime=0.0;
	//}
	//else if(temptime>0.5)
	//{
	//	m_rMainPlayer.SetCurrentSprID(15);
	//}
	//else
	//{
	//	m_rMainPlayer.SetCurrentSprID(m_rMainPlayer.GetLife());

	//}
}
