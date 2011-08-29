#pragma once
#include "FiniteState.h"
#include "FSM.h"
#include "ActHeroPig.h"

enum GESTateID
{
	GEStateID_Stop = 1,
	GEStateID_Run,
	GEStateID_Walk,
	GEStateID_Jump,
};

class CGEState:public CState
{
public:
	virtual void Walkto();
	virtual int ID(){return m_StateID;};
	CGEState(ActHeroPig& rEntity,int ID):m_rEntity(rEntity),m_StateID(ID){};
protected:

	ActHeroPig& Entity(){return m_rEntity;};
	CMovementEX& MovementEX(){return m_rEntity.GetMovement();};
	int m_StateID;
protected:
	ActHeroPig& m_rEntity;


};

class CGEStateRun :public CGEState
{
	//onstart里面出发了 _changeXYZandMove()
public:
	CGEStateRun(ActHeroPig& rEntity,int ID):CGEState(rEntity,ID){};
public:
	virtual void  On_Start();
	bool Transition(IEvent& rEvent)
	{
		if(rEvent.ID() == EventID_StartRun)
		{
			On_Start();
		}
		else if(rEvent.ID() == EventID_StartWalk)
		{
			On_Start();
		}
		else if(rEvent.ID() == EventID_StopWalk)
		{

			if(m_rEntity.GetKeyInput().m_x!=0 || m_rEntity.GetKeyInput().m_z!=0 )
				On_Start();
			else
				m_rEntity.GetControl().Jump(GEStateID_Stop);
		}
		else if (rEvent.ID() == EventID_StartJump)
		{
			m_rEntity.GetControl().Jump(GEStateID_Jump);
		}

			
		return true;
	}
	//virtual void  Walkto();
};

class CGEStateWalk :public CGEState
{
public:
	CGEStateWalk(ActHeroPig& rEntity,int ID):CGEState(rEntity,ID){};
	virtual ~CGEStateWalk(){};
	virtual void  On_Start();
	bool Transition(IEvent& rEvent)
	{
		if(rEvent.ID() == EventID_StartWalk)
		{
			On_Start();
		}
		if(rEvent.ID() == EventID_StartRun)
		{
			m_rEntity.GetControl().Jump(GEStateID_Run);
		}
		else if(rEvent.ID() == EventID_StopWalk)
		{
			m_rEntity.GetControl().Jump(GEStateID_Stop);
		}
		else if (rEvent.ID() == EventID_StartJump)
		{
			m_rEntity.GetControl().Jump(GEStateID_Jump);
		}

		return true;
	}
private:

};


class CGEStateStop :public CGEState
{
public:
	CGEStateStop(ActHeroPig& rEntity,int ID):CGEState(rEntity,ID){};
	virtual ~CGEStateStop(){};
	virtual void  On_Start();
	bool Transition(IEvent& rEvent)
	{
		if(rEvent.ID() == EventID_StartRun )
		{
			m_rEntity.GetControl().Jump(GEStateID_Run);
		}
		else if(rEvent.ID() == EventID_StartWalk)
		{
			m_rEntity.GetControl().Jump(GEStateID_Walk);
		}
		else if (rEvent.ID() == EventID_StartJump)
		{
			m_rEntity.GetControl().Jump(GEStateID_Jump);
		}
		else if (rEvent.ID() == EventID_StopWalk)
		{
			On_Start();
		}
		return true;
	};
	//virtual void Transition();
private:

};

class CGEStateJump :public CGEState
{
public:
	CGEStateJump(ActHeroPig& rEntity,int ID):CGEState(rEntity,ID){};
	virtual ~CGEStateJump(){};
	virtual void  On_Start();
	virtual bool  Transition(IEvent& rEvent)
	{
 		if(rEvent.ID() == EventID_StopJump)
		{
			m_rEntity.GetControl().Jump(GEStateID_Stop);
		}
		else if(rEvent.ID() == EventID_StartWalk)
		{
			On_Start();
		}
		return true;
	}
private:
};