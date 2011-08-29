#pragma once
#include "FSM.h"
#include <string>
#include <map>
using namespace std;


struct Direction
{	
	Direction()
	{
		memset(this,0,sizeof(Direction));
	}
	bool m_bLeft;
	bool m_bRight;
	bool m_bUp;
	bool m_bDown;
};

struct KeyInput
{
	KeyInput()
	{
		memset(this,0,sizeof(KeyInput));
	};

	void ChangeDirection(int nKey)
	{

		switch(nKey)
		{
		case DirectionID_Left:
			m_x = -1;
			DirectionState.m_bLeft = true;
			break;
		case DirectionID_Right:
			m_x = 1;
			DirectionState.m_bRight = true;
			break;
		case DirectionID_Up:
			m_z = -1;
			DirectionState.m_bUp = true;
			break;
		case DirectionID_Down:
			m_z = 1;
			DirectionState.m_bDown = true;
			break;
		case DirectionID_StopLeft:
			m_x =  DirectionState.m_bRight?1:0;
			DirectionState.m_bLeft = false;
			break;
		case DirectionID_StopRight:
			m_x =  DirectionState.m_bLeft?-1:0;
			DirectionState.m_bRight = false;
			break;
		case DirectionID_StopUp:
			m_z =  DirectionState.m_bDown?1:0;
			DirectionState.m_bUp = false;
			break;
		case DirectionID_StopDown:
			m_z =  DirectionState.m_bUp?-1:0;
			DirectionState.m_bDown = false;
			break;
		case DirectionID_Jump:
			m_y = -1;
			break;
		default:
			break;
		}
	}
	bool IsWalk()
	{
		return (m_x||m_y||m_z)&&(!m_bJump);
	}
	bool m_bJump;
	bool m_bRun;
	char m_x;
	char m_y;
	char m_z;
	Direction DirectionState;
};

struct LastKeyState
{
	LastKeyState()
	{
		memset(this,0,sizeof(LastKeyState));
	};
	bool IsRun(int key,float Time)
	{
		 if (key == m_nKey && (Time - m_fTime < 0.5) )
		 {	
			return true;
		 }
		 m_nKey = key ;
		 m_fTime = Time;
		 return	false ;
	}
	int m_nKey;
	float m_fTime;
};

class CState
{
public:
	virtual ~CState(){};
	virtual int ID(){return 0;};
	virtual std::string Name(){return "";};
	virtual void On_Start(){};
	virtual void On_Process(){};
	virtual void On_Stop(){};
	virtual void On_Interrpt(){};
	virtual void On_Pause(){};
	virtual void On_Resume(){};
	virtual bool Transition(IEvent& rEvent){return false;};
	virtual void TransitionEX(int nEventID){};
	virtual void On_Trainstion(IEvent& rEvent){};//开始前的回调
	virtual void AddTrainsition(int nEventID,int nID){};
	virtual void AddTCompleteTrainsition(int nID){};
	virtual void AddSyncTransition(int nID){};
	typedef std::map<int,int> TransitionMap;
	virtual void SetTransitionMap(TransitionMap* pMapTranstion){};
	virtual bool IsComplete(){return true;};

};