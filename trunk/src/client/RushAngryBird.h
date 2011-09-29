#pragma once
#include "monsterbase.h"

class RushAngryBird :
	public MonsterBase
{
public:
	RushAngryBird(EntityBase& rPlayer);
	~RushAngryBird(void);
	virtual void Update(float dt);
	virtual void MouseMoveStop(float mouseX,float mouseY){};
	virtual void UpdateSpr();
private:
	EntityBase& m_rPlayer;
	bool m_bRush;
	bool m_bStun;
	bool m_bReady; 
	float m_fStunTime;
};
