#pragma once
#include "monsterbase.h"
////////////////////////////////////////////////////////////////////////////
////     阴冷的杀手,慢慢的接近猪脚,一旦被攻击了会分身为3个快速接近
////			
//////////////////////////////////////////////////////////////////////////
class SmallBird :
	public MonsterBase
{
public:
	SmallBird(list<MonsterBase*>& AngryBird,EntityBase& rPlayer);
	virtual ~SmallBird(void);
	void SetIsChild();
	virtual void Update(float dt);
	virtual void SetDamage(int nDamage);
	virtual void MouseMoveStop(float mouseX,float mouseY){};
private:
	list<MonsterBase*>& m_AngryBird;
	bool m_bAncestors; //第一代是慢速接近,第二代是快速接近
	EntityBase& m_rPlayer;
	bool m_bRush;
	float m_fUpdateSpeedTime; //更新追踪的频率
};
