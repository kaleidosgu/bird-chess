#include "../netclient/ClientSocketMgr.h"
#include "UIPlayer.h"
#include "UIPlayerFace.h"
#include "UICard.h"
#include "EntityFactory.h"

#include "../cardgame/cardgamemsg.h"
extern clinetnet::CClientSocketMgr g_CSM;
extern hgeFont* fnt;

#define GETCTRL(x) ((UICard*)m_PlayerUI->GetCtrl(x))
#define GETUITEXTCTRL(x) ((NONOhgeGUIText*)m_PlayerUI->GetCtrl(x))

const float fSendCardShowTime = 2.0f;
UIPlayer::UIPlayer(const std::string& sName):m_x(0.0),m_y(0.0),m_fScale(1.0f),m_bShow(false),m_fSendCardShowTime(-1.0f)
{
	m_PlayerFace = new UIPlayerFace(sName);
	m_ShowReadyAndCurPly = new NONOUIText(1,0,0,20,10,fnt);
	HTEXTURE tex = EntityFactory::Instance().GetUITEXTURE();
	m_SendCard = new UICard(1,648,48,145,210,tex,33,32);
}

UIPlayer::~UIPlayer()
{
	delete m_PlayerFace;m_PlayerFace =0;
}
void UIPlayer::Render()
{
	if(m_bShow)
	{
		m_PlayerFace->Render(m_x,m_y,m_fScale);
		m_ShowReadyAndCurPly->Render(m_x+90,m_y+80);

		if(m_fSendCardShowTime>0)
			m_SendCard->Render(m_x+100,m_y+100);
	}
}
void UIPlayer::Update(float dt)
{
	if(m_fSendCardShowTime>0)
		m_fSendCardShowTime -= dt;
}

void UIPlayer::SetScale(float fScale)
{
	m_fScale = fScale;
}
void UIPlayer::SetPlayerInfo(const std::string& sName,int nPlayerID)
{
	m_PlayerFace->SetName(sName);
	m_nPlayerID = nPlayerID;
	m_bShow = true;
}

void UIPlayer::SetCurrentPlayer(bool bIsCurrentPlayer)
{
	if(bIsCurrentPlayer)
	{
		m_ShowReadyAndCurPly->SetText("Thinking...");
	}
	else
	{
		m_ShowReadyAndCurPly->SetText("");
	}
};

void UIPlayer::SetBirdID(int nBirdID)
{
	m_nPlayerBirdID = nBirdID;
	m_PlayerFace->SetBirdID(m_nPlayerBirdID);
}

void UIPlayer::SetPlayerIndex(int nIndex)
{
	m_nPlayerIndex = nIndex;
	m_x = 30+nIndex*150;
	m_y = 16;
};

void UIPlayer::SetSendCard(int nCardType,int nCardInstruction,int nTargetCardType )
{
	m_SendCard->SetCardInfo(nCardType,nCardInstruction);
	m_fSendCardShowTime = fSendCardShowTime;
}


const char* UIPlayer::GetName()
{
	return m_PlayerFace->GetName();
};




MainUIPlayer::MainUIPlayer(std::string sName):UIPlayer(sName),m_nRoomID(0),nCurCardPos(-1),m_bIsMaster(false),m_fWinGame(-1.0f)
{
	//创建所有按钮 开始/准备    牌1 牌2 牌3 
	m_PlayerUI =new hgeGUI();
	UICard * pCardButton;
	HTEXTURE tex = EntityFactory::Instance().GetUITEXTURE();
	pCardButton = new UICard(ePlayCard1,648,48,145,210,tex,33,32);
	m_PlayerUI->AddCtrl(pCardButton);
	pCardButton = new UICard(ePlayCard2,648 + 150 ,48,145,210,tex,33,32);
	m_PlayerUI->AddCtrl(pCardButton);
	pCardButton = new UICard(ePlayCard3,648+ 150+ 150,48,145,210,tex,33,32);
	m_PlayerUI->AddCtrl(pCardButton);

	for(int i = 0;i<=ePlayCardType5-ePlayCardType1;i++ )
	{
		pCardButton = new UICard(ePlayCardType1+i,630+125*i,248,122,122,tex,688,837);
		pCardButton->SetCardInfo(i,0);
		pCardButton->SetVisable(false);
		m_PlayerUI->AddCtrl(pCardButton);
	}
	m_RoomIDText = new NONOhgeGUIText(eTextRoomID,648,45,20,10,fnt);
	m_RoomIDText->SetMode(HGETEXT_CENTER);
	pCardButton = new UICard(eBeginGame,167,100,88,43,tex,789,288);
	pCardButton->SetVisable(false);
	m_PlayerUI->AddCtrl(pCardButton);

	m_RoomIDText->SetMaxTextLen(cMAX_USERNAME_LEN);
	m_RoomIDText->SetText("Room");
	m_RoomIDText->GetString() = "Room";
	m_PlayerUI->AddCtrl(m_RoomIDText);

	pStrWinPrincess =  EntityFactory::Instance().CreateSprite("WinPrincess");
	//m_RoomIDText->SetMode(AL_CENTER);

	//pCardButton = new UICard(ePlayCard4,648+ 150+ 150+ 150,48,145,194,tex,708,8);
	//m_PlayerUI->AddCtrl(pCardButton);
}
MainUIPlayer::~MainUIPlayer()
{

}

void MainUIPlayer::Render()
{
	m_PlayerUI->Render();
	//m_RoomIDText->Render();
	if(m_fWinGame>0)
		pStrWinPrincess->Render(722,355);
	UIPlayer::Render();
}
void MainUIPlayer::Update(float dt)
{
	SendCard2Server(m_PlayerUI->Update(dt));
	if(m_fWinGame>0)
		m_fWinGame -= dt;
	UIPlayer::Update(dt);
}


void MainUIPlayer::SetWinGame()
{ 
	m_fWinGame = 10.0f;
};

void MainUIPlayer::SetPlayerCardInfo(int nPosID,int nType,int nInstruction)
{
	GETCTRL(nPosID+1)->SetCardInfo( nType,nInstruction);
}

void MainUIPlayer::SendCard2Server(int m_nCardID)
{
	if(m_nCardID<=ePlayCard3 && m_nCardID>0)
	{
		MSG_CARDGAME_C2S_UseCard card;
		card.nCardPos = m_nCardID - 1;
		card.nCardType = GETCTRL(m_nCardID)->GetCardType();
		
		if(card.nCardType == 5)
		{
			nCurCardPos = card.nCardPos;
			for(int i = 0;i<=ePlayCardType5-ePlayCardType1;i++ )
			{
				GETCTRL(ePlayCardType1+i)->SetVisable(true);
				GETCTRL(ePlayCardType1+i)->SetCardInfo(i, GETCTRL(m_nCardID)->GetCardInstruction());
			}
		}
		else
		{
			g_CSM.SendMsg(card);
			for(int i = 0;i<=ePlayCardType5-ePlayCardType1;i++ )
			{
				GETCTRL(ePlayCardType1+i)->SetVisable(false);
				
			}
		}		
	}
	else if(m_nCardID>ePlayCard3 && m_nCardID <eChangeRoom &&GETCTRL(m_nCardID)->GetVisable())
	{

		MSG_CARDGAME_C2S_UseCard card;
		card.nCardPos = nCurCardPos;
		card.nCardType = GETCTRL(m_nCardID)->GetCardType();
		g_CSM.SendMsg(card);


		for(int i = 0;i<=ePlayCardType5-ePlayCardType1;i++ )
		{
			GETCTRL(ePlayCardType1+i)->SetVisable(false);
		}
	}
	else if(m_nCardID == eChangeRoom)
	{
		int temp =  atoi(m_RoomIDText->GetString().c_str());

		if(temp != m_nRoomID)
		{
			MSG_CARDGAME_C2S_EnterRoom nMsg;
			nMsg.nRoomID = temp ;
			g_CSM.SendMsg(nMsg);
		}
	}
	else if (m_nCardID == eBeginGame)
	{
		if(m_bIsMaster)
		{	
			MSG_CARDGAME_C2S_StartGame msg;
			g_CSM.SendMsg(msg);
		}
	}
}

void MainUIPlayer::SetRoomID(int nRoomID)
{
	char temp[10];
	sprintf_s(temp,"Room:%d",nRoomID);
	m_RoomIDText->SetText(temp);
	m_nRoomID = nRoomID;
}

void MainUIPlayer::SetMaster(bool bIsMaster)
{
	m_bIsMaster = bIsMaster;
	GETCTRL(eBeginGame)->SetVisable(bIsMaster);
}
void MainUIPlayer::SetGameStart(bool bStart)
{
	//if(bStart)
	//{
	//	GETCTRL(eBeginGame)->SetVisable(false);
	//	m_fWinGame = -1.0f;
	//}
	//else if(m_bIsMaster)
	//{
	//		GETCTRL(eBeginGame)->SetVisable(true);
	//}
}

void MainUIPlayer::SetCurrentPlayer(bool bIsCurrentPlayer)
{
	for(int i = 0;i<=ePlayCard3-ePlayCard1;i++ )
	{
		GETCTRL(ePlayCard1+i)->SetCardState(bIsCurrentPlayer);
	}
	UIPlayer::SetCurrentPlayer(bIsCurrentPlayer);
}
