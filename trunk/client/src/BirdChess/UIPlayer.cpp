#include "UIPlayer.h"
#include "UIPlayerFace.h"

UIPlayer::UIPlayer(const std::string& sName)
{
	m_PlayerFace = new UIPlayerFace(sName);
}

UIPlayer::~UIPlayer()
{
	delete m_PlayerFace;m_PlayerFace =0;
}
void UIPlayer::Render()
{
	m_PlayerFace->Render();
}
void UIPlayer::Update(float dt)
{

}

void UIPlayer::SetScale(float fScale)
{

}
void UIPlayer::SetName(const std::string& sName)
{
	m_PlayerFace->SetName(sName);
}
void UIPlayer::SetBirdID(int nBirdID)
{
	m_nPlayerBirdID = nBirdID;
	m_PlayerFace->SetBirdID(m_nPlayerBirdID);
}



MainUIPlayer::MainUIPlayer(std::string sName):UIPlayer(sName)
{
	//创建所有按钮 开始/准备    牌1 牌2 牌3 
}
MainUIPlayer::~MainUIPlayer()
{

}

void MainUIPlayer::Render()
{
	UIPlayer::Render();
}
void MainUIPlayer::Update(float dt)
{
	UIPlayer::Update(dt);
}

void MainUIPlayer::SendCard2Server(int m_nCardID)
{
	
}