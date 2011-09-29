#include "CardCmd.h"
#include "UIBirdEntity.h"
#include "UICloudEntity.h"
#include "BirdCardDefine.h"

using namespace std;
bool CardCmdBirdGO::Execute(std::vector<CCloudEntity*>& vecCloud,std::vector<CBirdEntity*>& m_vecBird)
{
	bool bRes(false);
	vector<CBirdEntity*>::iterator itBegin = m_vecBird.begin();
	vector<CBirdEntity*>::iterator itEnd = m_vecBird.end();
	for(;itBegin!=itEnd;++itBegin)
	{
		CBirdEntity* pBird = *itBegin;
		if(pBird->GetBirdType() == m_nBirdType && pBird->GetCloudIndex() < int(vecCloud.size()-1)) //一般是1,但是如果已经到10了就肯定不能再移动了
		{
			if(pBird->GetCloudIndex() >= 0)
				vecCloud[pBird->GetCloudIndex()]->PopBird(pBird);
			pBird->MoveTo(pBird->GetX() +CloudDis,vecCloud[pBird->GetCloudIndex()+1]->GetHeightIncludeBird());
			vecCloud[pBird->GetCloudIndex()+1]->PushBird(pBird);
			bRes= true;
			break;
		}
	}
	return bRes;
}


bool CardCmdBirdBACK::Execute(std::vector<CCloudEntity*>& vecCloud,std::vector<CBirdEntity*>& m_vecBird)
{
	bool bRes(false);
	vector<CBirdEntity*>::iterator itBegin = m_vecBird.begin();
	vector<CBirdEntity*>::iterator itEnd = m_vecBird.end();
	for(;itBegin!=itEnd;++itBegin)
	{
		CBirdEntity* pBird = *itBegin;
		if(pBird->GetBirdType() == m_nBirdType && pBird->GetCloudIndex() >= 1) 
		{
			vecCloud[pBird->GetCloudIndex()]->PopBird(pBird);
			pBird->MoveTo(pBird->GetX() - CloudDis,vecCloud[pBird->GetCloudIndex()-1]->GetHeightIncludeBird());
			vecCloud[pBird->GetCloudIndex()-1]->PushBird(pBird);
			bRes= true;
			break;
		}
	}
	return bRes;
}

bool CardCmdSlowBirdGO::Execute(std::vector<CCloudEntity*>& vecCloud,std::vector<CBirdEntity*>& m_vecBird)
{
	bool bRes(false);
	vector<CCloudEntity*>::iterator itBegin = vecCloud.begin();
	vector<CCloudEntity*>::iterator itEnd = vecCloud.end();
	for(;itBegin!=itEnd;++itBegin)
	{
		CCloudEntity* pCloud = *itBegin;
		if(!pCloud->GetBirdList().empty())
		{
			CBirdEntity* pBird = pCloud->GetBirdList().front();
			CardCmdBirdGO GoCmd(pBird->GetBirdType());
			GoCmd.Execute(vecCloud,m_vecBird);
			break;
		}
			
	}
	return bRes;
}



bool CardCmdFastBirdBack::Execute(std::vector<CCloudEntity*>& vecCloud,std::vector<CBirdEntity*>& m_vecBird)
{
	bool bRes(false);
	vector<CCloudEntity*>::reverse_iterator itBegin = vecCloud.rbegin();
	vector<CCloudEntity*>::reverse_iterator itEnd = vecCloud.rend();
	for(;itBegin!=itEnd;++itBegin)
	{
		CCloudEntity* pCloud = *itBegin;
		if(!pCloud->GetBirdList().empty())
		{
			CBirdEntity* pBird = pCloud->GetBirdList().front();
			CardCmdBirdBACK GoCmd(pBird->GetBirdType());
			GoCmd.Execute(vecCloud,m_vecBird);
			break;
		}
	}
	return bRes;
}


bool CardCmdBirdGOGO::Execute(std::vector<CCloudEntity*>& vecCloud,std::vector<CBirdEntity*>& m_vecBird)
{
	bool bRes(false);
	vector<CBirdEntity*>::iterator itBegin = m_vecBird.begin();
	vector<CBirdEntity*>::iterator itEnd = m_vecBird.end();
	for(;itBegin!=itEnd;++itBegin)
	{
		CBirdEntity* pBird = *itBegin;
		if(pBird->GetBirdType() == m_nBirdType ) 
		{
			if(pBird->GetCloudIndex() < int(vecCloud.size()-2))
			{
				if(pBird->GetCloudIndex()>= 0)
					vecCloud[pBird->GetCloudIndex()]->PopBird(pBird);
				pBird->MoveTo(pBird->GetX() + CloudDis*2,vecCloud[pBird->GetCloudIndex()+2]->GetHeightIncludeBird());
				vecCloud[pBird->GetCloudIndex()+2]->PushBird(pBird);
			}
			else if(pBird->GetCloudIndex() < int(vecCloud.size()-1))
			{
				if(pBird->GetCloudIndex()>= 0)
					vecCloud[pBird->GetCloudIndex()]->PopBird(pBird);
				pBird->MoveTo(pBird->GetX() + CloudDis,vecCloud[pBird->GetCloudIndex()+1]->GetHeightIncludeBird());
				vecCloud[pBird->GetCloudIndex()+1]->PushBird(pBird);
			}
			bRes= true;
			break;
		}
	}
	return bRes;
}


bool CardCmdBirdBACKBACK::Execute(std::vector<CCloudEntity*>& vecCloud,std::vector<CBirdEntity*>& m_vecBird)
{
	bool bRes(false);
	vector<CBirdEntity*>::iterator itBegin = m_vecBird.begin();
	vector<CBirdEntity*>::iterator itEnd = m_vecBird.end();
	for(;itBegin!=itEnd;++itBegin)
	{
		CBirdEntity* pBird = *itBegin;
		if(pBird->GetBirdType() == m_nBirdType ) 
		{
			if(pBird->GetCloudIndex() >= 2)
			{
				vecCloud[pBird->GetCloudIndex()]->PopBird(pBird);
				pBird->MoveTo(pBird->GetX() - CloudDis*2,vecCloud[pBird->GetCloudIndex()-2]->GetHeightIncludeBird());
				vecCloud[pBird->GetCloudIndex()-2]->PushBird(pBird);
			}
			else if(pBird->GetCloudIndex() >= 1)
			{
				vecCloud[pBird->GetCloudIndex()]->PopBird(pBird);
				pBird->MoveTo(pBird->GetX() - CloudDis,vecCloud[pBird->GetCloudIndex()-1]->GetHeightIncludeBird());
				vecCloud[pBird->GetCloudIndex()-1]->PushBird(pBird);
			}
			bRes= true;
			break;
		}
	}
	return bRes;
}
