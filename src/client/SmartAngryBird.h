#pragma once
#include "monsterbase.h"

class SmartAngryBird :
	public MonsterBase
{
public:
	SmartAngryBird(EntityBase& rPlayer);
	~SmartAngryBird(void);
	virtual void Update(float dt);
	virtual void UpdateSpr();
private:
	EntityBase& m_rPlayer;
	bool m_bRush;
	float m_fUpdateSpeedTime; //����׷�ٵ�Ƶ��
};
