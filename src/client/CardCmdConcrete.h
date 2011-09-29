/************************************************************************/
/*			卡牌动作产生器,由ID生产相应的动作                           */
/************************************************************************/

#pragma once

class CardCmdBase;
class CardCmdConcrete 
{
	CardCmdConcrete(){};
public:
	static CardCmdConcrete& Instance();
	~CardCmdConcrete(){};
	CardCmdBase* CreateCardCmd(int nCardType,int nCardInstruction,int nTargetCardType );
};