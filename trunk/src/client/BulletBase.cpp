#include "BulletBase.h"
#include "GameDefine.h"


BulletBase::BulletBase(void):scale(1)
{
}

BulletBase::~BulletBase(void)
{
}

void BulletBase::Update(float dt)
{
	SetLocation(dx*dt + m_x,m_y+dy*dt);
	if(m_x>SCREEN_WIDTH || m_x<0) { SetEntityStat(ENUM_ENTITY_NOTALIVE);G_nCombo =0;};
	if(m_y>SCREEN_HEIGHT || m_y<0) {SetEntityStat(ENUM_ENTITY_NOTALIVE);G_nCombo =0;}; //pBullets[i].dy=-pBullets[i].dy;
}
 

void BulletBase::SetState(float nRotion,float xSpeed,float ySpeed,float beginx,float beginy,bool bPower)
{
	SetLocation(beginx,beginy);
	rot = nRotion;
	dx= xSpeed;
	dy= ySpeed;
	//if(bPower)
	//{
	//	scale = 1.3;
	//}
	//else
	//{
	//	scale = 1.3;
	//}

}

void BulletBase::Render()
{
	RenderEx(m_x, m_y, rot,scale);
}
//bool BulletBase::CaculateLocation()
//{
//	
//}
void BulletBase::ChangeSpeed(float fSpeedRate)
{
	dx *= fSpeedRate;
	dy *= fSpeedRate;
}