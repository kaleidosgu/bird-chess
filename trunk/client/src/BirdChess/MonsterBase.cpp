#include "MonsterBase.h"
#include "GameDefine.h"
#include <math.h>

MonsterBase::MonsterBase(void):m_bStop(false),m_bChangeSpeed(false)
{
	m_dColorValue = 0xFFD6002D;
	InitSpeed(60,150);
	SetScore(12);
	m_fDesotryTime = 0.1f;
} 


MonsterBase::~MonsterBase(void)
{
	
}

void MonsterBase::InitSpeed(int nMin,int nMax)
{
	dx = rand()%nMax - nMin;
	dy = rand()%nMax - nMin;
	dx>0?dx+=nMin:dx-=nMin;
	dy>0?dy+=nMin:dy-=nMin;
}

void MonsterBase::UpdateSpr()
{
	if(m_bStop)
	{
		SetCurrentSprID(1);
	}
	else if(GetCurrentSprID() == 1)
	{
		SetCurrentSprID(0);
	}

}
void MonsterBase::Update(float dt)
{
	if(IsAlive())
	{
		UpdateSpr();
		if(!m_bStop)
		{
			SetLocation(dx*dt + m_x,m_y+dy*dt);
			if(m_x>SCREEN_WIDTH || m_x<0) dx = -dx;
			if(m_y>SCREEN_HEIGHT || m_y<0) dy = -dy;
			dx>0?m_bFaceRight = true:m_bFaceRight = false;
		}
		else
		{
			//GetCurrentSpr()->SetFlip(!(dx>0),false,false);
			dx>0?m_bFaceRight = true:m_bFaceRight = false;
		}
		GetCurrentSpr()->SetFlip(!m_bFaceRight,false,false);	
	}
	
	EntityBase::Update(dt);
}

void MonsterBase::MouseMoveStop(float mouseX,float mouseY)
{
	static float s_fRange = pow(m_fWidth/2,2);
	float distant = pow(GetX() -mouseX,2) + pow(GetY() - mouseY,2);
	if(distant >= s_fRange)
	{ 
		m_bStop = false;
	}else 
	{
		m_bStop =true;
	}
}

bool MonsterBase::IsPlayerTouch(float fPlayerX,float fPlayerY,float fPlayerSize)
{
	static float s_fPlayerRange = pow(m_fWidth/2,2)+pow(fPlayerSize,2);
	float distant = pow(GetX() -fPlayerX,2) + pow(GetY() - fPlayerY,2);
	if (distant >= s_fPlayerRange)
	{
		return false;
	}
	return true;
}

void MonsterBase::ChangeSpeed(float fSpeedRate,bool bCheck)
{
	if(bCheck)
	{
		if(m_bChangeSpeed)
		{
			dx *= fSpeedRate;
			dy *= fSpeedRate;
			m_bChangeSpeed = false;
		}
	}
	else
	{
		if(m_bChangeSpeed == false)
		{
			dx *= fSpeedRate;
			dy *= fSpeedRate;
			m_bChangeSpeed = true;
		}
	}
}