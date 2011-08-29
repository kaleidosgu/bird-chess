#include "StrongAngryBird.h"

StrongAngryBird::StrongAngryBird(void)
{
	SetLife(2);//Á½µãÑªµÄÄñÄñ
	InitSpeed(20,50);
	SetScore(20);
	m_dColorValue = 0xFF000000;
}

StrongAngryBird::~StrongAngryBird(void)
{
}


void StrongAngryBird::SetDamage(int nDamage)
{
	SetCurrentSprID(2);
	EntityBase::SetDamage(nDamage);
}
