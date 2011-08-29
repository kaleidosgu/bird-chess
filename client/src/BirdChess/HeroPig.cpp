#include "HeroPig.h"
#include "GameDefine.h"
#include "BombBullet.h"
#include "EntityFactory.h"

#include <math.h>

#define PLAYER_POWER_TIME 6

HeroPig::HeroPig(void):m_fdx(0.0),m_fdy(0.0),m_fChangeBulletRation(0.0),m_nPlayerStat(ENUM_PLAY_NORMAL),m_nPower(100),m_fShootCD(0.2),m_fPowerTimeLeft(0.0),MainPar(NULL),m_UseBombCD(0),m_fScale(1)
{
	SetLocation(500,900);//初始位置
	m_fAddSpeed = cMainPlayAddSpeed;
	SetLife(nDefaultLife);
}

HeroPig::~HeroPig(void)
{
	delete pPowUseSpr;
}

void HeroPig::Update(float dt)
{
	m_UseBombCD -= dt;
	if(m_fPowerTimeLeft > 0.0)
	{
		m_fPowerTimeLeft -= dt;
		//ainPar->info.nEmission=(int)(GetDX()*GetDX()+GetDY()*GetDY())*2;
		MainPar->MoveTo(GetRendX()+rand()%20 -10,GetRendY()+rand()%20 -10);
		MainPar->Update(dt);
	}
	else
	{
		SetShootCD(0.2);
		SetChangeBulletRation(0);
	}
	m_fdx*=friction; m_fdy*=friction; m_x+=m_fdx; m_y+=m_fdy;
	if(m_x>=SCREEN_WIDTH-(m_fWidth/2)) {m_x=SCREEN_WIDTH-(m_fWidth/2);m_fdx=-m_fdx;boom();}
	if(m_x<m_fWidth/2) {m_x=m_fWidth/2+m_fWidth/2-m_x;m_fdx=-m_fdx;boom();}
	if(m_y>SCREEN_HEIGHT -m_fHeight/2) {m_y=SCREEN_HEIGHT-m_fHeight/2;m_fdy=-m_fdy;boom();}
	if(m_y<m_fHeight/2) {m_y=m_fHeight/2+m_fHeight/2-m_y;m_fdy=-m_fdy;boom();}
}

void HeroPig::ChangeSpeed(float ChangeX,float ChangeY)
{
	m_fdx+= ChangeX*m_fAddSpeed;
	m_fdy+= ChangeY*m_fAddSpeed;
}
void HeroPig::boom()
{

}

void HeroPig::SetMousePosition(float fx,float fy)
{
	if(fx-m_x>0)
	{
		GetCurrentSpr()->SetFlip(false,false,false);
	}
	else
	{
		GetCurrentSpr()->SetFlip(true,false,false);
		//rotion  = M_PI + atan((MouseY-y)/(MouseX-x));//因为翻转了所以不用这么干了
	}

	m_fRation = atan((fy-m_y)/(fx-m_x));
}

void HeroPig::Render()
{
	if(m_fPowerTimeLeft > 0.0&& MainPar)
	{
		MainPar->Render();
	}
	RenderEx(m_x,m_y,m_fRation,m_fScale);
}

void HeroPig::SetLife(int nLife)
{
	nLife = min(8,nLife);
	if(GetCurrentSprID()<=10)
	{
		if(GetLife()>=8)
			SetCurrentSprID(nLife);
		else
			SetCurrentSprID(nLife);
	}

	EntityBase::SetLife(nLife);
}

float HeroPig::GetBulletRation()
{
	if(m_fChangeBulletRation > 0)
		return m_fRation+(rand()%100)*m_fChangeBulletRation*0.01-m_fChangeBulletRation/2;
	else
		return m_fRation;
};

BulletBase* HeroPig::CreateBullet(float mouseX,float mouseY)
{
	int nBullet =  NORM_SPEED_BULETS;
	//if(m_fPowerTimeLeft > 0.0)
	//{
		//nBullet = 1000;
	//}
	float fRation = GetBulletRation();
	float xSpeed =abs(cos(fRation)*nBullet); 
	float ySpeed = abs(sin(fRation)*nBullet);
	if(mouseX - m_x < 0) { xSpeed =-xSpeed ;fRation = M_PI+fRation;}
	if(mouseY - m_y< 0) ySpeed =-ySpeed ;
	BulletBase* pBullet = new BulletBase();
	if(m_nBulletLife ==2)
	{
		pBullet = EntityFactory::Instance().CreateBulletByName("BulletDouble");
		PlaySound(22,70);
	}
	else if(m_fPowerTimeLeft > 0.0)
	{
		pBullet = EntityFactory::Instance().CreateBulletByName("BulletPower");
		PlaySound(21,70);
	}
	else
	{
		PlaySound(21,70);
		pBullet = EntityFactory::Instance().CreateBulletByName("BulletNormal");
		//pBullet->SetTexture(m_tex,134,944,30,23);

	}
	pBullet->SetState(fRation,xSpeed,ySpeed,m_x,m_y,false);
	pBullet->SetLife(m_nBulletLife);
	return pBullet;
}

void HeroPig::UsePower()
{

	if(m_nPower >= PLAYER_MAX_POWER)
	{
		m_nPower = 0;
		m_fPowerTimeLeft+= PLAYER_POWER_TIME;
		SetShootCD(0.1);
		SetChangeBulletRation(1);
		MainPar=new hgeParticleSystem("Res/FontPsi/PowerUse.psi",GetCurrentSpr());//
		MainPar->Fire();
		PlaySound(31,150);
	}
}
BulletBase* HeroPig::UseBomb(int nType)
{
	BulletBase* pBullet = NULL;
	if(m_UseBombCD<= 0)
	{
		pBullet = new BombBullet();
		
		nType = nType%4;
		if(nType == 0)
		{
			pBullet = EntityFactory::Instance().CreateBulletByName("BulletBomb1",true);
		}
		else if(nType == 1)
			pBullet = EntityFactory::Instance().CreateBulletByName("BulletBomb2",true);

		else if(nType == 2)
			pBullet = EntityFactory::Instance().CreateBulletByName("BulletBomb3",true);
		else 
			pBullet = EntityFactory::Instance().CreateBulletByName("BulletBomb4",true);



		pBullet->GetCurrentSpr()->SetColor(0xFF6DE249);
		pBullet->SetState(0,rand()%200-100,rand()%200-100,m_x+rand()%50-25,m_y+rand()%50-25,false);
		pBullet->SetScale((nType+1)*0.4);
	}
	return pBullet;
}