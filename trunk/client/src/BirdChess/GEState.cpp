//#include "stdafx.h"
#include "GEState.h"
#include "TimeRoute.h"

void CGEStateRun::On_Start()
{
	CPointState* pWalkState = new CWalkPointState();
	pWalkState->SetSpeed(INT3DPoint(Entity().GetXSpeed()*4,0,Entity().GetZSpeed()*4)); 
	pWalkState->SetTime(Entity().GetTime());
	//pWalkState->SetBeginPos())
	Entity().GetMovement().Append(pWalkState);
	Entity().GetKeyInput().m_bRun = true;
	Entity().SetCurrentSprID(5);
}
void CGEState::Walkto()
{
	//MovementEX().DeafultMove(int nX,int nY,int nZ)
}


void  CGEStateWalk::On_Start()
{
	CPointState* pWalkState = new CWalkPointState();
	pWalkState->SetSpeed(INT3DPoint(Entity().GetXSpeed(),0,Entity().GetZSpeed())); 
	pWalkState->SetTime(Entity().GetTime());
	//pWalkState->SetBeginPos())
	Entity().GetMovement().Append(pWalkState);
	Entity().GetKeyInput().m_bRun = false;
	Entity().SetCurrentSprID(3);
}



// void CGEStateStop::Transition()
// {
// 	int nPoset = 0;
// }


void CGEStateStop::On_Start()
{
 	if(Entity().GetKeyInput().IsWalk())
 		m_rEntity.GetControl().Jump(GEStateID_Walk);
 	else
	{
		CPointState* pWalkState = new CWalkPointState();
		pWalkState->SetSpeed(INT3DPoint(0,0,0)); 
		pWalkState->SetTime(Entity().GetTime());
		Entity().GetMovement().Append(pWalkState);
		Entity().SetCurrentSprID(2);
	}
	
}

void CGEStateJump::On_Start()
{
	Entity().SetCurrentSprID(8);
	if(!Entity().GetKeyInput().m_bJump)
	{
		CPointState* pWalkState = new CJumpPointState(); 
		if(Entity().GetKeyInput().m_bRun)
			pWalkState->SetSpeed(INT3DPoint(Entity().GetXSpeed()*3,Entity().GetYSpeed(),Entity().GetZSpeed()*3)); 
		else
			pWalkState->SetSpeed(INT3DPoint(Entity().GetXSpeed(),Entity().GetYSpeed(),Entity().GetZSpeed())); 
		pWalkState->SetTime(Entity().GetTime());
		Entity().GetMovement().Append(pWalkState);
		Entity().GetKeyInput().m_bJump = true;
	}
	else
	{
		CPointState* pWalkState = new CJumpPointState();
		CPointState* ResJumpState = Entity().GetMovement().GetLastPs();
		if(Entity().GetKeyInput().m_bRun)
			pWalkState->SetSpeed(INT3DPoint(Entity().GetXSpeed()*3,ResJumpState->GetSpeed().y,Entity().GetZSpeed()*2)); 
		else
			pWalkState->SetSpeed(INT3DPoint(Entity().GetXSpeed()*3,ResJumpState->GetSpeed().y,Entity().GetZSpeed())); 
		pWalkState->SetTime(Entity().GetTime());
		Entity().GetMovement().Append(pWalkState);
	}
	CPointState* pWalkState = new CJumpPointState();

}


