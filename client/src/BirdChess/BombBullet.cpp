#include "BombBullet.h"

BombBullet::BombBullet(void)
{
	SetLife(1000);
	scale = 1;
	dscale = 0.5;
}

BombBullet::~BombBullet(void)
{
}

void BombBullet::Update(float dt)
{
	scale += dscale*dt*scale;
	rot += 3*dt;
	if(scale>3)
	{
		SetDamage(1000);
	}
	SetLocation(dx*dt + m_x,m_y+dy*dt);
}