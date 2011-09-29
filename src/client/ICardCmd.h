/*************************************************************************/
/*	���Ʋ���,���� �����һ,�����˶�,������5,�ѿ��Ʋ�����װ����			 */
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