#include "CreateEggBird.h"
#include "GameDefine.h"
#include "EntityFactory.h"

CreateEggBird::CreateEggBird(list<MonsterBase*>& AngryBird):m_AngryBird(AngryBird),m_TimePass(0),m_bDamaged(false)
{
	m_dColorValue = 0xFFFFFFFF;
	InitSpeed(60,70);
}

CreateEggBird::~CreateEggBird(void)
{
}
void CreateEggBird::SetDamage(int nDamage)
{
	EntityBase::SetDamage(nDamage);
	SetCurrentSprID(GetLife());
	m_TimePass = 0.0;
	dx *=-3;
	dy *=-3;
	m_bDamaged = true;
	
}


void CreateEggBird::MouseMoveStop(float mouseX,float mouseY)
{
	
}

void CreateEggBird::Update(float dt)
{
	if(IsAlive())
	{
		m_TimePass += dt;
		if(m_TimePass > 4)
		{
			if(m_bDamaged)
			{
				dx /= 3;
				dy /= 3;
				m_bDamaged = false;
			}
			m_TimePass = 0.0;
			SetLife(GetLife()+1);
		}
		if(GetLife()>=3)
		{
			SetLife(1);
			MonsterBase* pBird = EntityFactory::Instance().CreateBird(ENUM_BIRDTYPE_EGGBIRD,m_AngryBird);
			pBird->SetLocation(GetX(),GetY());
			m_AngryBird.push_back(pBird);
		}
	}
	//UpdateSpr();
	MonsterBase::Update(dt);
}

void CreateEggBird::SetLife(int nLife)
{
	if(GetLife())
		SetCurrentSprID(nLife);
	else
		SetCurrentSprID(nLife);
	EntityBase::SetLife(nLife);
}