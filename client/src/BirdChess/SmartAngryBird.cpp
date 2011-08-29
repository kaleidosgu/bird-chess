#include "SmartAngryBird.h"
#include <math.h>
#define SMARTBIRD_SPEED 300

SmartAngryBird::SmartAngryBird(EntityBase& rPlayer):m_rPlayer(rPlayer),m_fUpdateSpeedTime(1.0),m_bRush(false)
{
	m_dColorValue = 0xFFF1DB20;
} 


SmartAngryBird::~SmartAngryBird(void)
{
}

void SmartAngryBird::Update(float dt)
{
	if(m_fUpdateSpeedTime>1)
	{
		float xDistants = m_rPlayer.GetX() - GetX() ;
		float yDistants = m_rPlayer.GetY() - GetY() ;
		float fDistants = pow(float(xDistants*xDistants+yDistants*yDistants),float(0.5));

		int nAddSpeed = rand()%200;
		dx = xDistants*SMARTBIRD_SPEED /fDistants  + nAddSpeed - 100;
		dy = yDistants*SMARTBIRD_SPEED /fDistants + nAddSpeed - 100;
		if(m_bChangeSpeed)
		{
			dx*= 0.2f;
			dy*= 0.2f;
		}
	    if(abs( dx) + abs(dy)  > 500)
		{
			m_bRush = true;
		}
		else
		{
			m_bRush = false;
		}
		m_fUpdateSpeedTime = 0.0;
	}
	else
	{
		m_fUpdateSpeedTime += dt;
	}
	MonsterBase::Update(dt);
}

void SmartAngryBird::UpdateSpr()
{
	if(m_bStop)
	{
		SetCurrentSprID(1); 
	}
	else if(m_bRush)
	{
		SetCurrentSprID(2); 
	}else
	{
		SetCurrentSprID(0); 
	}
}