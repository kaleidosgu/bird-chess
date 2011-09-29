#pragma  once
#include "TimeRoute.h"

class CMovementEX //移动的管理
{
public:
	CMovementEX(){
		m_Locate.x = 500;
		m_Locate.y = 0;
		m_Locate.z= 900;
	};
	~CMovementEX(){};
	void Append(CPointState* pPs)
	{
		TimeRoute.AppendRouteStep(pPs);
	}
	CPointState* GetLastPs()
	{
		return TimeRoute.GetPointState();
	}
	void Heartbeat(float fTime)
	{
		m_Locate = m_Locate+TimeRoute.CalcutePoint(fTime);
	};
	void GetLocate(float& x,float& y,float& z)
	{
		x= m_Locate.x;
		y= m_Locate.y;
		z= m_Locate.z;
	};
private:
	INT3DPoint m_Locate;
	CTimeRoute TimeRoute;

};