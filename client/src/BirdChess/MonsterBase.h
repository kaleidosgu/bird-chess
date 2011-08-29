#pragma once
#include "EntityBase.h"

class MonsterBase :
	public EntityBase
{
public:
	MonsterBase(void);
	virtual ~MonsterBase(void);
	virtual void Update(float dt);
	virtual void UpdateSpr();
	virtual void MouseMoveStop(float mouseX,float mouseY);
	bool IsPlayerTouch(float fPlayerX,float fPlayerY,float fPlayerSize);
	void ChangeSpeed(float mSpeedRate,bool bCheck = false);
	DWORD GetColor() { return m_dColorValue;};
	int GetScore() {return m_nScore;};
	void SetScore(int nScore) {m_nScore = nScore;};
protected:
	virtual void InitSpeed(int nMin,int nMax);
	float dx,dy;
	bool m_bStop;
	DWORD m_dColorValue;
	bool m_bChangeSpeed;
private:
	int m_nScore;
};
