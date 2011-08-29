#pragma once
#include "EntityBase.h"
#include "hgeparticle.h"
class BulletBase;
class HeroPig :
	public EntityBase
{
public:
	HeroPig(void);
	~HeroPig(void);
	virtual void Update(float dt);
	virtual void ChangeSpeed(float dx,FLOAT dy); //这里的参数传变量
	virtual void Render();
	virtual void SetLife(int nLife);

	//float GetRation(){return sm_fRation;};
	float GetBulletRation();
	float GetAddSpeed(){return m_fAddSpeed;};
	int GetPower(){return m_nPower;};
	float GetShootCD(){return m_fShootCD;};
	void SetShootCD(float fShootCD){ m_fShootCD = fShootCD;};
	void SetPower(int nPower){ m_nPower = nPower;};
	void SetChangeBulletRation(float fRation){ m_fChangeBulletRation = fRation;};
	void SetAddSpeed(float fAddSpeed){ m_fAddSpeed = fAddSpeed;};
	void SetRation(float fRation){ m_fRation = fRation; };
	void SetMousePosition(float fx,float fy);
	float GetDX(){return m_fdx;};
	float GetDY(){return m_fdy;};
	void boom();
	
	void SetPlayerStat(int nPlayerStat){m_nPlayerStat = nPlayerStat;};
	int GetPlayerStat(){return m_nPlayerStat;};

	BulletBase* CreateBullet(float mouseX ,float mouseY);
	void UsePower();
	BulletBase* UseBomb(int nType);

	void SetBulletLife(int nBulletLife){m_nBulletLife = nBulletLife;};
	void SetBombCD(float fBombCD){m_UseBombCD = fBombCD;};
	float GetBombCD(){return m_UseBombCD;};
	void SetScale(float fScale){m_fScale = fScale;};
private:
	int m_nPlayerStat; 
	float m_fRation;
	float m_fChangeBulletRation;
	float m_fdx;
	float m_fdy;
	float m_fAddSpeed;
	int m_nPower;
	float m_fShootCD;
	float m_fPowerTimeLeft;
	int m_nBulletLife;
	float m_UseBombCD;
	hgeParticleSystem* MainPar;
	hgeSprite* pPowUseSpr;
	float m_fScale;
};

