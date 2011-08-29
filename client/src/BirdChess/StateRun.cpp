//#include "stdafx.h"
#include "StateRun.h"
#include "TimeRoute.h"

void CGEStateRun::On_Start()
{
	CGEState::On_Start();
	//MovementEX().EnabeContinueMove();
	int nPosX =0;
	int nPosY =0;
	int nPosZ =0;
	//MovementEX().GetPosition(nPosX,nPosY,nPosZ);
	Walkto();
}
void CGEState::Walkto()
{
	//MovementEX().DeafultMove(int nX,int nY,int nZ)
}


void  CGEStateWalk::On_Start()
{
	CPointState* pWalkState = new CWalkPointState();
	pWalkState->SetSpeed(INT3DPoint(1,0,0));
	pWalkState->SetTime(0);
	//pWalkState->SetBeginPos())
	Entity().GetMovement().Append(pWalkState);
}



void CGEStateStop::Transition()
{
	int nPoset = 0;
}

void CGEStateStop::On_Start()
{

}