//////////////////////////////////////////////////////////////////////////
// һ����,n���ֺ���������...������ֻ������ֵ�
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
