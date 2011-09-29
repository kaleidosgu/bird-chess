#pragma once
#include "entitybase.h"

class ItemBase :
	public EntityBase
{
public:
	ItemBase(void);
	virtual ~ItemBase(void);
	virtual void ItemEffect();
	virtual void BeforRemoveItemEffect(float fdt){};
	virtual void RemoveItemEffect(){};
	bool IsPlayerTouch(float fPlayerX,float fPlayerY,float fPlayerSize);
	virtual void Update(float dt);
	virtual int GetItemState() {return m_nItemState;};
	virtual void Render();
	void SetEffectTime(float fTime){ m_fEffectTime = fTime;};
	void SetLeftTime(float fTime){ m_fLeftTime = fTime;};
protected:
	int m_nItemState;
	static int m_nChangeRoleSpr;
private:
	float m_fEffectTime;
	float m_fLeftTime;

};
