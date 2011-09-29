#pragma once
#include "heropig.h"
#include "IMoveable.h"
#include "FSM.h"
#include "TimeRoute.h"
#include "Movement.h"
#include "FiniteState.h"


class ActHeroPig :
	public HeroPig,public CMoveable
{
public:
	ActHeroPig(void);

	//ActHeroPig(CFSM_T<CState>& rFSMControl):m_rFSMControl(rFSMControl)
	//{
	//}
	virtual ~ActHeroPig(void);
	virtual void Update(float fTime){ Heartbeat(fTime); };
	void Response(IEvent& rEvent)
	{
		keyState.ChangeDirection(rEvent.GetKey());
		m_rFSMControl->Response(rEvent);
	}
	
	void Heartbeat(float fTime)
	{ 
		m_fTime = fTime;
		m_Movement.Heartbeat(fTime); 
		m_Movement.GetLocate(m_x,m_y,m_z);
		if(m_y > 0)
		{
			keyState.m_y = 0;
			keyState.m_bJump =false;
			CGEEvent State(EventID_StopJump);
			m_rFSMControl->Response(State);
			m_y =0;
		}
		SetLocation(m_x,(m_y+m_z));
	};
	
	CMovementEX& GetMovement(){return m_Movement;};
	CState* GetState(){m_rFSMControl->CurrentStatePtr();};
	CFSM_T<CState>& GetControl(){return *m_rFSMControl;};
	KeyInput& GetKeyInput(){return keyState;};
	float GetTime(){return m_fTime;};
	float GetXSpeed(){return m_xSpeed*(keyState.m_x); };
	float GetYSpeed(){return m_ySpeed*(keyState.m_y); };
	float GetZSpeed(){return m_zSpeed*(keyState.m_z);};
private:
	CMovementEX m_Movement;
	CFSM_T<CState>* m_rFSMControl;
	float m_z;
	KeyInput keyState;
	float m_fTime;

	float m_xSpeed;
	float m_ySpeed;
	float m_zSpeed;


};
