#include "RushAngryBird.h"
#include "GameDefine.h"
#include <math.h>
#define RUSHBIRD_SPEED 240

RushAngryBird::RushAngryBird(EntityBase& rPlayer):m_rPlayer(rPlayer),m_bStun(true),m_fStunTime(5)
{
	m_dColorValue = 0xFF129D49;
}

RushAngryBird::~RushAngryBird(void)
{
}


void RushAngryBird::Update(float dt)
{
	if(IsAlive())
	{
		if(!m_bStun)
		{
			SetLocation(dx*dt + m_x,m_y+dy*dt);
			(dx>0)?m_bFaceRight = true:	m_bFaceRight = false;
			if(m_x>SCREEN_WIDTH-RUSHBIRDS_MOUSESIZE || m_x<RUSHBIRDS_MOUSESIZE) m_bStun=true;// dx = -dx;
			if(m_y>SCREEN_HEIGHT || m_y<0) dy = -dy;
			UpdateSpr();
		}
		else if(m_fStunTime >= 3)
		{
			m_bStun = false;
			m_fStunTime = 0.0;
			float xDistants = m_rPlayer.GetX() - GetX() ;
			float yDistants = m_rPlayer.GetY() - GetY() ;
			float fDistants = pow(float(xDistants*xDistants+yDistants*yDistants),float(0.5));

			//int nAddSpeed = rand()%200;
			dx = xDistants*RUSHBIRD_SPEED /fDistants;//  + nAddSpeed - 100;
			dy = yDistants*RUSHBIRD_SPEED /fDistants;// + nAddSpeed - 100;
			m_bChangeSpeed = false;//�ٶ�������
			m_bFaceRight = (dx>0.0);
			SetLocation(dx*dt + m_x,m_y+dy*dt);

		}
		else
		{
			m_fStunTime += dt;
			if(m_fStunTime > 2 && m_bReady == false) //3������ѣ��׼��ʱ��
			{
				m_bReady = true;
				m_bFaceRight = !m_bFaceRight; //ֱ�ӷ���
			}
			else
				m_bReady = false;
		}
	}
	UpdateSpr();
	EntityBase::Update(dt);
}

void RushAngryBird::UpdateSpr()
{
	if(IsAlive())
	{	
		if(m_bStun)
		{
			if(m_bReady)
				SetCurrentSprID(0);  //׼��״̬
			else
			{
				SetCurrentSprID(1);  //��ѣ
			}
		}
		else 
		{
			SetCurrentSprID(2);   //���
		}

	}
	GetCurrentSpr()->SetFlip(!m_bFaceRight,false,false);	
}