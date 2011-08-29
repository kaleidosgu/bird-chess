#include "AddLifeItem.h"

AddLifeItem::AddLifeItem(EntityBase& rMainPlayer):m_rMainPlayer(rMainPlayer)
{
	m_fDesotryTime = 0.0;
}

AddLifeItem::~AddLifeItem(void)
{
}


void AddLifeItem::ItemEffect()
{
	m_rMainPlayer.SetLife(m_rMainPlayer.GetLife()+1);
	PlaySound(35);
}