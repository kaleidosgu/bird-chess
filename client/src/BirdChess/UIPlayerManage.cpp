#include "UIPlayerManage.h"
#include "UIPlayer.h"

const int nMaxPlayerSize = 4;

using namespace std;
UIPlayerManage::UIPlayerManage()
{
	for(int i =0;i< nMaxPlayerSize;++i) //ÀÁºº´´½¨
	{
		if (i == 0)
		{
			MainUIPlayer* pPlayer  = new MainUIPlayer(" ");
			m_VecPlayer.push_back(pPlayer);
			m_MainPlayer = pPlayer;
		}
		else
		{
			UIPlayer* pPlayer  = new UIPlayer(" ");
			m_VecPlayer.push_back(pPlayer);
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

//void SetPlayerInfo()
//{
//
//}


//UIPlayerManage& UIPlayerManage::GetInstance()
//{
//	static UIPlayerManage m_StaticManage;
//	return m_StaticManage;
//}

