#pragma once
#include "BulletBase.h"

class BombBullet :
	public BulletBase
{
public:
	BombBullet();
	virtual ~BombBullet(void);

	virtual void Update(float dt);

};
