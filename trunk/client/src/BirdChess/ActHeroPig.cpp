#include "ActHeroPig.h"
#include "GEState.h"

 ActHeroPig::ActHeroPig(void)
 {
	static CFSM_T<CState> Container(10);
	CGEStateRun*  pStateRun= new CGEStateRun(*this,GEStateID_Run);
	CGEStateWalk*  pStateWalk= new CGEStateWalk(*this,GEStateID_Walk);
	CGEStateStop* pStateStop = new CGEStateStop(*this,GEStateID_Stop);
	CGEStateJump* pStateJump = new CGEStateJump(*this,GEStateID_Jump);
	Container.Add(pStateRun,pStateRun->ID());
	Container.Add(pStateWalk,pStateWalk->ID());
	Container.Add(pStateStop,pStateStop->ID());
	Container.Add(pStateJump,pStateJump->ID());
	
	Container.Start(GEStateID_Stop);
	Container.Refresh();
	m_rFSMControl = &Container;

	 m_xSpeed = 50;
	 m_ySpeed = 500;
	 m_zSpeed = 50;

 }

ActHeroPig::~ActHeroPig(void)
{
}




// void ActHeroPig::ResponseMove(int Event)
// {
// // 	switch(Event)
// // 	{
// // 		case:
// // 	}
// 		
// }
