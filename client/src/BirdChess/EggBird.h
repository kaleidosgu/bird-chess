//////////////////////////////////////////////////////////////////////////
// 一个蛋,n秒种后会孵化成鸟...这个鸟又会下这种蛋
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "monsterbase.h"

class EggBird :
	public MonsterBase
{
public:
	EggBird(list<MonsterBase*>& AngryBird);
	virtual ~EggBird(void);
	void Update(float dt);

private:
	list<MonsterBase*>& m_AngryBird;
	float m_TimePass;
};
