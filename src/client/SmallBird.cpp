#include "SmallBird.h"
#include "GameDefine.h"
#include "EntityFactory.h"

#include <math.h>
#define BIGBIRD_SPEED 150
#define SMALLBIRD_SPEED 350
SmallBird::SmallBird(list<MonsterBase*>& AngryBird,EntityBase& rPlayer):m_AngryBird(AngryBird),m_rPlayer(rPlayer),m_fUpdateSpeedTime(0.5),m_bAncestors(true)
{
}

SmallBird::~SmallBird(void)
{
}

void SmallBird::SetIsChild()
{
	m_bAncestors = false;
	m_dColorValue = 0xFF63AAC5;
	SetCurrentSprID(2);
	SetLife(2);
}
void SmallBird::SetDamage(int nDamage)
{
	if(m_bAncestors)
	{
		MonsterBase* pBird = EntityFactory::Instance().CreateBird(ENUM_BIRDTYPE_SMALLBIRD,m_AngryBird,&m_rPlayer);
		pBird->SetLocation(GetX()+0.5*dx,GetY()+0.5*dy);
		((SmallBird*)pBird)->SetIsChild();//来强的了....
		m_AngryBird.push_back(pBird);
		pBird = EntityFactory::Instance().CreateBird(ENUM_BIRDTYPE_SMALLBIRD,m_AngryBird,&m_rPlayer);
		pBird->SetLocation(GetX()+0.3*dx,GetY());
		((SmallBird*)pBird)->SetIsChild();
		m_AngryBird.push_back(pBird);
		pBird = EntityFactory::Instance().CreateBird(ENUM_BIRDTYPE_SMALLBIRD,m_AngryBird,&m_rPlayer);
		pBird->SetLocation(GetX(),GetY()+0.3*dy);
		((SmallBird*)pBird)->SetIsChild();
		m_AngryBird.push_back(pBird);
	}
	EntityBase::SetDamage(nDamage);
}

void SmallBird::Update(float dt)
{
	if(m_fUpdateSpeedTime>0.5)
	{
		float xDistants = m_rPlayer.GetX() - GetX() ;
		float yDistants = m_rPlayer.GetY() - GetY() ;
		float fDistants = pow(float(xDistants*xDistants+yDistants*yDistants),float(0.5));
		if(m_bAncestors)
		{
			dx = xDistants*BIGBIRD_SPEED /fDistants ;
			dy = yDistants*BIGBIRD_SPEED /fDistants ;
		}
		else
		{
			dx = xDistants*SMALLBIRD_SPEED /fDistants ;
			dy = yDistants*SMALLBIRD_SPEED /fDistants ;
		}

		if(m_bChangeSpeed)
		{
			dx*= 0.2f;
			dy*= 0.2f;
		}
		m_fUpdateSpeedTime = 0.0;
	}
	else
	{
		m_fUpdateSpeedTime += dt;
	}
	MonsterBase::Update(dt);
}