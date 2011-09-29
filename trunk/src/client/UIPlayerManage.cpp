#include "UIPlayerManage.h"
#include "UIPlayer.h"
#include "UIShowMessage.h"
const int nMaxPlayerSize = 2;

using namespace std;
UIPlayerManage::UIPlayerManage():m_nMasterID(-1)
{
	for(int i =0;i< nMaxPlayerSize;++i) //懒汉创建
	{
		if (i == 0)
		{
			MainUIPlayer* pPlayer  = new MainUIPlayer("NONO");
			m_VecPlayer.push_back(pPlayer);
			pPlayer->SetPlayerIndex(i);
			m_MainPlayer = pPlayer;
		}
		else
		{
			UIPlayer* pPlayer  = new UIPlayer("SomeThingElse");
			m_VecPlayer.push_back(pPlayer);
			pPlayer->SetPlayerIndex(i);
			pPlayer->SetScale(0.7); //其他玩家头像较小
		}		
	
	}
}

UIPlayerManage::~UIPlayerManage()
{
	vector<UIPlayer*>::iterator itBeg = m_VecPlayer.begin();
	vector<UIPlayer*>::iterator itEnd = m_VecPlayer.end();
	for(;itBeg!=itEnd;++itBeg) 
	{
		delete *itBeg;	
	}
	m_VecPlayer.clear();
}

void UIPlayerManage::Render()
{
	vector<UIPlayer*>::iterator itBeg = m_VecPlayer.begin();
	vector<UIPlayer*>::iterator itEnd = m_VecPlayer.end();
	for(;itBeg!=itEnd;++itBeg) 
	{
		(*itBeg)->Render();	
	}
}


void UIPlayerManage::Update(float fTime)
{
	vector<UIPlayer*>::iterator itBeg = m_VecPlayer.begin();
	vector<UIPlayer*>::iterator itEnd = m_VecPlayer.end();
	for(;itBeg!=itEnd;++itBeg) 
	{
		(*itBeg)->Update(fTime);
	}
}

bool UIPlayerManage::IsMaster()
{ 
	return m_MainPlayer->GetPlayerID() == m_nMasterID;
};



void UIPlayerManage::SetCurrentPlayerID(int nCurrentPlayerID)
{
	m_nCurrentPlayerID = nCurrentPlayerID;
	vector<UIPlayer*>::iterator itBeg = m_VecPlayer.begin();
	vector<UIPlayer*>::iterator itEnd = m_VecPlayer.end();
	for(;itBeg!=itEnd;++itBeg) 
	{
		if((*itBeg)->GetPlayerID() == nCurrentPlayerID)
			(*itBeg)->SetCurrentPlayer(true);
		else
			(*itBeg)->SetCurrentPlayer(false);
	}
	if(m_MainPlayer->GetPlayerID() == nCurrentPlayerID)
	{
		CShowMessage::Instance().SetShowMesage("Your Turn Now!");
	}
};

void UIPlayerManage::SetRoomMater(int nMasterID)
{
	m_nMasterID = nMasterID;
	m_MainPlayer->SetMaster(m_MainPlayer->GetPlayerID() == m_nMasterID);
}

void UIPlayerManage::Response(int nCardType,int nCardInstruction,int nTargetCardType )
{
	vector<UIPlayer*>::iterator itBeg = m_VecPlayer.begin();
	vector<UIPlayer*>::iterator itEnd = m_VecPlayer.end();
	for(;itBeg!=itEnd;++itBeg) 
	{
		if((*itBeg)->GetPlayerID() == m_nCurrentPlayerID)
			(*itBeg)->SetSendCard(nCardType,nCardInstruction,nTargetCardType);
	}
}
//void SetPlayerInfo()
//{
//
//}


//UIPlayerManage& UIPlayerManage::GetInstance()
//{
//	static UIPlayerManage m_StaticManage;
//	return m_StaticManage;
//}


