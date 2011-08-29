#include "SlowBirdItem.h"
#include "Gamedefine.h"
#define FCHANGESPEED 0.2

using namespace std;

int SlowBirdItem::m_nChangeBG = 0;
SlowBirdItem::SlowBirdItem(std::list<BulletBase*>& rBulletlist,std::list<MonsterBase*>& rAngryBird):
m_rBulletlist(rBulletlist),
m_rAngryBird(rAngryBird)
{
	m_fDesotryTime = 0.5;
	SetEffectTime(15);
}

SlowBirdItem::~SlowBirdItem(void)
{

}

void SlowBirdItem::RemoveItemEffect()
{
	m_nChangeBG--;
	if(m_nChangeBG == 0)
	{
		G_bgPrintType = ENUM_BG_NEEDRESUME;
		list<MonsterBase*>::iterator itBegBird = m_rAngryBird.begin();
		list<MonsterBase*>::iterator itEndBird = m_rAngryBird.end();
		for(;itBegBird!= itEndBird;itBegBird++)
		{
			(*itBegBird)->ChangeSpeed(5,true);
		}

	}
	
	//list<BulletBase*>::iterator itBegBullet = m_rBulletlist.begin();
	//list<BulletBase*>::iterator itEndBullet = m_rBulletlist.end();
	//for(;itBegBullet!= itEndBullet;itBegBullet++)
	//{
	//	(*itBegBullet)->ChangeSpeed(5);
	//}
	
}
void SlowBirdItem::ItemEffect()
{	

	m_nChangeBG++;
	G_bgPrintType = ENUM_BG_BACK;
	list<MonsterBase*>::iterator itBegBird = m_rAngryBird.begin();
	list<MonsterBase*>::iterator itEndBird = m_rAngryBird.end();
	for(;itBegBird!= itEndBird;itBegBird++)
	{
		(*itBegBird)->ChangeSpeed(FCHANGESPEED);
	}

	list<BulletBase*>::iterator itBegBullet = m_rBulletlist.begin();
	list<BulletBase*>::iterator itEndBullet = m_rBulletlist.end();
	for(;itBegBullet!= itEndBullet;itBegBullet++)
	{
		(*itBegBullet)->ChangeSpeed(FCHANGESPEED);
	}

	ItemBase::ItemEffect();
	PlaySound(33);
}


void SlowBirdItem::BeforRemoveItemEffect(float fdt)
{
	//static float temptime(0.0);
	//temptime +=fdt;

	//if(temptime>1)
	//{
	//	temptime=0.0;
	//}

	//else if(temptime>0.5)
	//{
	//	m_rbg.SetBlendMode(BLEND_COLORMUL | BLEND_ALPHABLEND | BLEND_NOZWRITE);
	//}
	//else
	//{
	//	m_rbg.SetBlendMode(BLEND_COLORADD | BLEND_ALPHABLEND | BLEND_NOZWRITE);
	//}
}