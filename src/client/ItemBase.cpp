#include "ItemBase.h"
#include "GameDefine.h"
#include <math.h>

int ItemBase::m_nChangeRoleSpr = 0;
ItemBase::ItemBase(void):m_nItemState(ENUM_ITEM_WAIT)
{
	SetLeftTime(10.0);
	SetEffectTime(5.0);
	m_fDesotryTime = 0.0;
	PlaySound(14); 
}

ItemBase::~ItemBase(void)
{
}

bool ItemBase::IsPlayerTouch(float fPlayerX,float fPlayerY,float fPlayerSize)
{
	static float s_fPlayerRange = pow(m_fWidth/2,2)+pow(fPlayerSize,2);
	float distant = pow(GetX() -fPlayerX,2) + pow(GetY() - fPlayerY,2);
	if (distant >= s_fPlayerRange)
	{
		return false;
	}
	return true;
}

void ItemBase::Update(float dt)
{
	if(m_nItemState == ENUM_ITEM_WAIT)
	{
		m_fLeftTime -= dt;
 		if(m_fLeftTime <= 0 )	
		{
			m_nItemState = ENUM_ITEM_TIMEOVER;
		}
	}
	if(m_nItemState == ENUM_ITEM_EFFECT)
	{
		m_fEffectTime -= dt;

		if(m_fEffectTime<=0)
		{
			RemoveItemEffect();
			m_nItemState = ENUM_ITEM_TIMEOVER;
		}
		else if(m_fEffectTime<=3)
		{
			BeforRemoveItemEffect(dt);
		}
	}
	EntityBase::Update(dt);
}

void ItemBase::ItemEffect()
{ 
	m_nItemState = ENUM_ITEM_EFFECT; 
}

void ItemBase::Render()
{
	if(m_nItemState == ENUM_ITEM_WAIT || m_fDesotryTime > 0)
	{
		EntityBase::Render();
	}
}