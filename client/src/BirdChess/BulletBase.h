#pragma once
#include "EntityBase.h"

class BulletBase :
	public EntityBase
{
public:
	void SetState(float nRotion,float xSpeed,float ySpeed,float beginx,float beginy,bool bPower);
	virtual void Update(float dt);
	virtual void Render();
	float GetScale(){return scale;};
	void SetScale(float fscale){scale = fscale;};
	void ChangeSpeed(float fSpeedRate);
	BulletBase(void);
	~BulletBase(void);

protected:
	float scale,rot;
	float dscale,drot;
	float dx,dy;

};
