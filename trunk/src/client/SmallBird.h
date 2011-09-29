#pragma once
#include "monsterbase.h"
////////////////////////////////////////////////////////////////////////////
////     �����ɱ��,�����Ľӽ����,һ���������˻����Ϊ3�����ٽӽ�
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
	bool m_bAncestors; //��һ�������ٽӽ�,�ڶ����ǿ��ٽӽ�
	EntityBase& m_rPlayer;
	bool m_bRush;
	float m_fUpdateSpeedTime; //����׷�ٵ�Ƶ��
};
