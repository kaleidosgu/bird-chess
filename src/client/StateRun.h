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
	bool Transition(IEvent& rEvent)
	{
		if(rEvent.ID() == EventID_StartRun )
		{
			return true;
		}
		if(rEvent.ID() == EventID_StartWalk)
		{
			m_rEntity.GetControl().Jump(GEStateID_Walk);
		}
	};
protected:

	ActHeroPig& Entity(){return m_rEntity;};
	CMovementEX& MovementEX(){return m_rEntity.m_Movement;};
	int m_StateID;
private:
	ActHeroPig& m_rEntity;


};

class CGEStateRun :public CGEState
{
	//onstart里面出发了 _changeXYZandMove()
public:
	CGEStateRun(ActHeroPig& rEntity,int ID):CGEState(rEntity,ID){};
public:
	virtual void  On_Start();
	//virtual void  Walkto();
};

class CGEStateWalk :public CGEState
{
public:
	CGEStateWalk(ActHeroPig& rEntity,int ID):CGEState(rEntity,ID){};
	virtual ~CGEStateWalk(){};
	virtual void  On_Start();
private:

};


class CGEStateStop :public CGEState
{
public:
	CGEStateStop(ActHeroPig& rEntity,int ID):CGEState(rEntity,ID){};
	virtual ~CGEStateStop(){};
	virtual void  On_Start();
	virtual void Transition();
private:

};

class CGEStateJump :public CGEState
{
public:
	CGEStateJump(ActHeroPig& rEntity,int ID):CGEState(rEntity,ID){};
	virtual ~CGEStateJump(){};
	virtual void  On_Start();
	virtual void  Transition();
private:
};