/*************************************************************************/
/*	卡牌操作,例如 二鸟进一,三鸟退二,慢鸟退5,把卡牌操作封装起来			 */
/*																	     */
/*************************************************************************/
#pragma once 
#include <vector>

class CCloudEntity;
class CBirdEntity;

class CardCmdBase
{
public:
	CardCmdBase(){};
	virtual ~CardCmdBase(){};
	virtual bool Execute(std::vector<CCloudEntity*>& vecCloud,std::vector<CBirdEntity*>& m_vecBird)=0;
};