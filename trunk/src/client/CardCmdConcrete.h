/************************************************************************/
/*			���ƶ���������,��ID������Ӧ�Ķ���                           */
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