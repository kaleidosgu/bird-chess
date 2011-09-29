/************************************************************************/
/* 卡牌具体操作                                                          */
/************************************************************************/

#pragma once 

#include "ICardCmd.h"
//鸟1进1
class CardCmdBirdGO :public CardCmdBase
{
public:
	CardCmdBirdGO(int nBirdType):m_nBirdType(nBirdType){};
	virtual ~CardCmdBirdGO(){};
	virtual bool Execute(std::vector<CCloudEntity*>& vecCloud,std::vector<CBirdEntity*>& m_vecBird);
private:
	int m_nBirdType;
};

class CardCmdBirdBACK :public CardCmdBase
{
public:
	CardCmdBirdBACK(int nBirdType):m_nBirdType(nBirdType){};
	virtual ~CardCmdBirdBACK(){};
	bool Execute(std::vector<CCloudEntity*>& vecCloud,std::vector<CBirdEntity*>& m_vecBird);
private:
	int m_nBirdType;
};


class CardCmdSlowBirdGO :public CardCmdBase
{
public:
	CardCmdSlowBirdGO(){};
	virtual ~CardCmdSlowBirdGO(){};
	bool Execute(std::vector<CCloudEntity*>& vecCloud,std::vector<CBirdEntity*>& m_vecBird);

};

class CardCmdFastBirdBack :public CardCmdBase
{
public:
	CardCmdFastBirdBack(){};
	virtual ~CardCmdFastBirdBack(){};
	bool Execute(std::vector<CCloudEntity*>& vecCloud,std::vector<CBirdEntity*>& m_vecBird);
};

//
class CardCmdBirdGOGO :public CardCmdBase
{
public:
	CardCmdBirdGOGO(int nBirdType):m_nBirdType(nBirdType){};
	virtual ~CardCmdBirdGOGO(){};
	bool Execute(std::vector<CCloudEntity*>& vecCloud,std::vector<CBirdEntity*>& m_vecBird);
private:
	int m_nBirdType;
};

class CardCmdBirdBACKBACK :public CardCmdBase
{
public:
	CardCmdBirdBACKBACK(int nBirdType):m_nBirdType(nBirdType){};
	virtual ~CardCmdBirdBACKBACK(){};
	bool Execute(std::vector<CCloudEntity*>& vecCloud,std::vector<CBirdEntity*>& m_vecBird);
private:
	int m_nBirdType;
};

