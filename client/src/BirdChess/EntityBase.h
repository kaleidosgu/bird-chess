//////////////////////////////////////////////////////////////////////////
//
// ��Ϸ�ж���Ļ���
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
	//void play();//����
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
	HTEXTURE m_tex;//�������������Ķ���
	float m_fDesotryTime;
private:
	int  m_nEntityStat;
	map<int,hgeSprite*> m_sprMap;    // 0-10 ������֡������   10-20������֡������  20-30����������
	hgeSprite* m_sprDebug;//�����˺����
	int m_CurrentSprID; //��0��ʼ
	int m_nLife;	//����ֵ
};
