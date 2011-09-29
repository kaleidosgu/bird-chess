#pragma once
#include "monsterbase.h"
////////////////////////////////////////////////////////////////////////////
//��С���µ���,���ڽ���͵͵�µ�
//////////////////////////////////////////////////////////////////////////
class CreateEggBird :
	public MonsterBase
{
public:
	CreateEggBird(list<MonsterBase*>& AngryBird);
	virtual ~CreateEggBird(void);
	void Update(float dt);
	virtual void SetLife(int nLife);
	virtual void SetDamage(int nDamage);
	virtual void MouseMoveStop(float mouseX,float mouseY);
private:
	list<MonsterBase*>& m_AngryBird;
	float m_TimePass;
	bool m_bDamaged;
};
