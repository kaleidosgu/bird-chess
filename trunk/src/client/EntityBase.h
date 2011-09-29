//////////////////////////////////////////////////////////////////////////
//
// 游戏中对象的基类
//
//////////////////////////////////////////////////////////////////////////
#pragma once

#include "hge.h"
#include "hgesprite.h"
#include <map>
#include <list>

using namespace std;

struct EntitySize;
class EntityBase
{
public:
	EntityBase(void);
	virtual ~EntityBase(void);
	//void play();//播放
	virtual void Update(float dt);
	virtual void UpdateSpr();

	void SetTexture(HTEXTURE tex,float fLeftupX,float fLeftupY,float fWidth,float fHeight,unsigned int nID = 0);
	void SetTexture(HTEXTURE tex,list<EntitySize*>& listEntitySize);
	void SetCurrentSprID(int nID) ;
	int GetCurrentSprID(){return m_CurrentSprID;};
	virtual void Render();
	void Render(float x, float y);
	void RenderEx(float x, float y, float rot, float hscale=1.0f, float vscale=0.0f);
	void SetLocation(float X,float Y);
	bool IsAlive();
	void PlaySound(int nID,int nPre=100);	
	void SetEntityStat(int nAlive) { m_nEntityStat = nAlive;}
	int GetEntityStat() { return m_nEntityStat;};
	float GetX(){return m_x;}
	float GetY(){return m_y;}
	void SetOrderX(float fOrdx){ m_LogicOrdx= fOrdx;};
	void  SetOrderY(float fOrdy){ m_LogicOrdy=fOrdy; };
	float GetWidth(){ return m_fWidth;};
	float GetHeight(){ return m_fHeight;};
	virtual void SetDamage(int nDamage);
	int GetLife(){ return m_nLife;};
	virtual void SetLife(int nLife){ m_nLife = nLife;};
	virtual void OnDestory();
	virtual float GetRendX() {return m_x - m_LogicOrdx;};
	virtual float GetRendY() {return m_y - m_LogicOrdy;};
//protected:
	hgeSprite* GetCurrentSpr();
protected:
	float m_x;
	float m_y;

	static float m_LogicOrdx;
	static float m_LogicOrdy;
	
	float m_fWidth;
	float m_fHeight;
	bool m_bFaceRight;
	//int m_nDestoryStat;
	HTEXTURE m_tex;//用来创建其他的东西
	float m_fDesotryTime;
private:
	int  m_nEntityStat;
	map<int,hgeSprite*> m_sprMap;    // 0-10 是正常帧数区域   10-20是死亡帧数区域  20-30是特殊区域
	hgeSprite* m_sprDebug;//调试伤害框框
	int m_CurrentSprID; //从0开始
	int m_nLife;	//生命值
};
