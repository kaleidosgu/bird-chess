#pragma once
#include "monsterbase.h"

class StrongAngryBird :
	public MonsterBase
{
public:
	StrongAngryBird(void);
	~StrongAngryBird(void);
	virtual void SetDamage(int nDamage);
	//virtual void InitSpeed();
};
