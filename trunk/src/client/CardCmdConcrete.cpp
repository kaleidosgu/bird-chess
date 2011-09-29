#include "CardCmd.h"
#include "CardCmdConcrete.h"
#include "../cardgame/card.h"

CardCmdBase* CardCmdConcrete::CreateCardCmd(int nCardType,int nCardInstruction,int nTargetCardType )
{
	//CardType_Red,
	//	CardType_Blue,
	//	CardType_Green,
	//	CardType_Purple,
	//	CardType_Yellow,
	//	CardType_MultiColour,
	//	CardType_Unknown,
	CardCmdBase* pCardCmd = NULL;
	switch (nCardInstruction)   //卡牌总张是一致的,这里要和服务器逻辑一致并解析
	{

	case Back_2:
		pCardCmd =new CardCmdBirdBACKBACK(nTargetCardType);
		break;
	case Back_1:
		pCardCmd =new CardCmdBirdBACK(nTargetCardType);
		break;
	case Forward_1:
		pCardCmd =new CardCmdBirdGO(nTargetCardType);
		break;
	case Forward_2:
		pCardCmd =new CardCmdBirdGOGO(nTargetCardType);
		break;
	//case 1:
	//	pCardCmd =new CardCmdBirdGO(0);
	//	break;
	//case 2:
	//	pCardCmd =new CardCmdBirdGO(4);
	//	break;
	//case 3:
	//	pCardCmd =new CardCmdBirdGO(3);
	//	break;
	//case 4:
	//	pCardCmd =new CardCmdSlowBirdGO();
	//	break;
	//case 5:
	//	pCardCmd =new CardCmdBirdBACK(0);
	//	break;
	//case 6:
	//	pCardCmd =new CardCmdBirdBACK(4);
	//	break;
	//case 7:
	//	pCardCmd =new CardCmdBirdBACK(3);
	//	break;
	//case 8:
	//	pCardCmd =new CardCmdFastBirdBack();
	//	break;
	//case 9:
	//	pCardCmd =new CardCmdBirdGOGO(1);
	//	break;
	//case 10:
	//	pCardCmd =new CardCmdBirdBACKBACK(1);
	//	break;
	//default:
	//	break;
	}
	return pCardCmd;
}

CardCmdConcrete& CardCmdConcrete::Instance()
{
	static CardCmdConcrete Factory;
	return Factory;
}