/////////////////////////////////////
//	轨迹:根据时间可以获得当前的xyz
//
/////////////////////////////////////

#pragma once
#include <deque>

#define MAXPSSIZE 10
struct INT3DPoint
{
	INT3DPoint()
	{
		x = 0;
		y = 0;
		z = 0;
	}
	INT3DPoint(float fx,float fy,float fz)
	{
		x = fx;
		y = fy;
		z = fz;
	}
	float x;
	float y;
	float z;
	INT3DPoint operator *(float fTime)
	{
		INT3DPoint temp;
		temp.x = this->x * fTime;
		temp.y = this->y* fTime;
		temp.z = this->z* fTime;
		return temp;
	}
	void operator =(INT3DPoint& src)
	{
		this->x = src.x;
		this->y = src.y;
		this->z = src.z;
	}

	INT3DPoint operator +(INT3DPoint& src)
	{
		INT3DPoint temp;
		temp.x = this->x + src.x;
		temp.y = this->y + src.y;
		temp.z = this->z + src.z;
		return temp;
	}
};


class CPointState
{
public:
	CPointState():m_t(0){};
	virtual INT3DPoint CalcutePoint(float fTime)
	{
		INT3DPoint aaa;
		aaa.x = 1;
		aaa.y = 1;
		aaa.z = 1;
		return aaa;
	}
	virtual void SetSpeed(INT3DPoint Speed){m_Speed = Speed;};
	virtual void SetTime(float ftime){m_t = ftime;};
	virtual void SetBeginPos( INT3DPoint BeginPos) { m_BeginPos = BeginPos; };
	virtual INT3DPoint GetSpeed(){return m_Speed;};
protected:
	INT3DPoint m_Speed;
	INT3DPoint m_BeginPos;//起始
	float m_t;//起始时间
};

class CWalkPointState:public CPointState
{
public:
	virtual INT3DPoint CalcutePoint(float fTime)
	{
		return  m_Speed*(fTime);
	}
};

class CJumpPointState:public CPointState
{
public:
	CJumpPointState():m_zhongli(0,700,0)
	{
		
	}
	virtual INT3DPoint CalcutePoint(float fTime)
	{
		 m_Speed = m_Speed+m_zhongli*fTime;
		 return m_Speed *fTime;
	}
	INT3DPoint m_zhongli;
};

class CTimeRoute //轨迹的管理
{
public:
	CTimeRoute(){};
	~CTimeRoute()
	{
		for(deque<CPointState*>::iterator itBeg = m_vecPS.begin();itBeg!=m_vecPS.end();++itBeg)
		{
			delete *itBeg;
		}
		m_vecPS.clear();
	};
	bool AppendRouteStep(CPointState* pPs)
	{ 
		m_vecPS.push_back(pPs);
		if( m_vecPS.size()> MAXPSSIZE)
		{
			m_vecPS.pop_front();
		}
		return true;
	}
	CPointState* GetPointState(int nIndex){};
	CPointState* GetPointState(){return m_vecPS.back();};
	INT3DPoint CalcutePoint(float fTime,CPointState* pPs){pPs->CalcutePoint(fTime);};
	INT3DPoint CalcutePoint(float fTime)
	{
		INT3DPoint temp;
		if(!m_vecPS.empty())
		{
			CPointState* pPs = m_vecPS.back();
			temp = pPs->CalcutePoint(fTime);
		}
		return temp;
	}
	///INT3DPoint CalcutePoint(float fTime){ }
private:
	deque<CPointState*> m_vecPS;
};