#include "../netclient/ClientSocketMgr.h"
#include "UIGround.h"
#include "UICloudEntity.h"
#include "UIBirdEntity.h"
#include "EntityFactory.h"
#include "BirdCardDefine.h"
#include "CardCmdConcrete.h"
#include "ICardCmd.h"
#include "UIPlayerManage.h"
#include "UIPlayer.h"
#include "../cardgame/cardgamemsg.h"
#include "UIShowMessage.h"

const int nMaxBirdCount = 5;
const int nMaxCloudCount = 10;
const int nFirstBirdY = 180;
const int nFirstBirdX = 30;
const int nBirdDisY = 100;
const int nCloudY = 650;

extern clinetnet::CClientSocketMgr g_CSM;
using namespace std;


CUIGround::CUIGround():m_bReady(false)
{
	for(int i = 0 ;i < nMaxBirdCount ; ++i)
	{
		CBirdEntity* pEntity = EntityFactory::Instance().CreateUIBird(i);
		m_vecBird.push_back(pEntity);
		pEntity->SetLocation(nFirstBirdX,nBirdDisY*i+nFirstBirdY);
	}
	//m_vecBird[2]->SetNext(m_vecBird[3]) ;
	for(int i = 0 ;i < nMaxCloudCount ; ++i)
	{
		CCloudEntity* pEntity = EntityFactory::Instance().CreateUICloud(i);
		m_vecCloud.push_back(pEntity);
		pEntity->SetLocation(CloudDis*i,nCloudY);
	}	
	m_PlayerManager = new UIPlayerManage();
}

CUIGround::~CUIGround()
{
	vector<CBirdEntity*>::iterator itBeginBird = m_vecBird.begin();
	vector<CBirdEntity*>::iterator itEndBird = m_vecBird.end();
	for(;itBeginBird!=itEndBird;++itBeginBird)
	{
		delete *itBeginBird;
	}
	m_vecBird.clear();

	vector<CCloudEntity*>::iterator itBeginCloud = m_vecCloud.begin();
	vector<CCloudEntity*>::iterator itEndCloud = m_vecCloud.end();
	for(;itBeginBird!=itEndBird;++itBeginBird)
	{
		delete *itBeginBird;
	}
	m_vecCloud.clear();
	delete m_PlayerManager;m_PlayerManager =0;
}

void CUIGround::Render()
{
	if(m_bReady)
	{
		vector<CBirdEntity*>::iterator itBeginBird = m_vecBird.begin();
		vector<CBirdEntity*>::iterator itEndBird = m_vecBird.end();
		for(;itBeginBird!=itEndBird;++itBeginBird)
		{
			(*itBeginBird)->Render();
		}

		vector<CCloudEntity*>::iterator itBeginCloud = m_vecCloud.begin();
		vector<CCloudEntity*>::iterator itEndCloud = m_vecCloud.end();
		for(;itBeginCloud!=itEndCloud;++itBeginCloud)
		{
			(*itBeginCloud)->Render();
		}
		m_PlayerManager->Render();
	}

	/*std::vector<CBirdEntity*> m_vecBird;
	std::vector<CCloudEntity*> m_vecCloud;
	std::map<int,std::list<CBirdEntity*> > m_mapCloundState;
	for(m_vecBird)*/
}

void CUIGround::Update(float dt)
{
	if(m_bReady)
	{
		vector<CBirdEntity*>::iterator itBeginBird = m_vecBird.begin();
		vector<CBirdEntity*>::iterator itEndBird = m_vecBird.end();
		for(;itBeginBird!=itEndBird;++itBeginBird)
		{
			(*itBeginBird)->Update(dt);
		}

		vector<CCloudEntity*>::iterator itBeginCloud = m_vecCloud.begin();
		vector<CCloudEntity*>::iterator itEndCloud = m_vecCloud.end();
		for(;itBeginCloud!=itEndCloud;++itBeginCloud)
		{
			(*itBeginCloud)->Update(dt);
		}
		
		if(!m_vecCardCmd.empty() && CBirdEntity::GetBirdMoveAble())
		{
			m_vecCardCmd.back()->Execute(m_vecCloud,m_vecBird);
			m_vecCardCmdback.push_front(m_vecCardCmd.back());
			m_vecCardCmd.pop_back();

			if(m_vecCardCmdback.size()>50)
			{
				delete m_vecCardCmdback.back();
				m_vecCardCmdback.pop_back();
			}
		}
		m_PlayerManager->Update(dt);
	}

}

void CUIGround::Response(int nCardType,int nCardInstruction,int nTargetCardType )
{
	CardCmdBase* pCardCmd = CardCmdConcrete::Instance().CreateCardCmd(nCardType,nCardInstruction,nTargetCardType);
	m_vecCardCmd.push_front(pCardCmd);
	m_PlayerManager->Response(nCardType,nCardInstruction,nTargetCardType );
}

void CUIGround::SetMainPlayerInfo(MSG_CARDGAME_S2C_PlayerInfo& rPlayerInfo)
{
	m_PlayerManager->GetMainPlayer()->SetPlayerInfo(rPlayerInfo.szPlayerName,rPlayerInfo.nPlayerID);
}

void CUIGround::SetRoomMater(int nID)
{
	m_PlayerManager->SetRoomMater(nID);
}
void CUIGround::PlayerLeave(int nID)
{
	std::vector<UIPlayer*>&  rPlayerVec =  m_PlayerManager->GetPlayerList();
	rPlayerVec[1]->SetVisiable(false);
	//rPlayerVec[1]->SetPlayerInfo(rPlayerInfo.szPlayerName,rPlayerInfo.nPlayerID);		//目前只支持两个人 0 和 1
}

void CUIGround::SetPlayerInfo(MSG_CARDGAME_S2C_PlayerInfo& rPlayerInfo)
{
		//m_PlayerManager->SetPlayerInfo(rPlayerInfo.szPlayerName,rPlayerInfo.nPlayerID);
		std::vector<UIPlayer*>&  rPlayerVec =  m_PlayerManager->GetPlayerList();
		if(rPlayerInfo.nPlayerID!= m_PlayerManager->GetMainPlayer()->GetPlayerID())
		{
			rPlayerVec[1]->SetPlayerInfo(rPlayerInfo.szPlayerName,rPlayerInfo.nPlayerID);		//目前只支持两个人 0 和 1
		}
}

void CUIGround::SetPlayerCardInfo(int nPosID,int nType,int nInstruction)
{
	 m_PlayerManager->GetMainPlayer()->SetPlayerCardInfo(nPosID,nType,nInstruction);
}


void CUIGround::SetPlayerBirdID(int nPlayerCardType)
{
	 m_PlayerManager->GetMainPlayer()->SetBirdID(nPlayerCardType);
}

void CUIGround::SetCurrentTurnPlayer(int nPlayerID)
{
	m_PlayerManager->SetCurrentPlayerID(nPlayerID);
}

void CUIGround::SetOrderY(float fOrdy)
{
	m_vecBird[0]->SetOrderY(fOrdy);
	m_vecCloud[0]->SetOrderY(fOrdy);
}

void CUIGround::SetOrderX(float fOrdx)
{
	m_vecBird[0]->SetOrderX(fOrdx);
	m_vecCloud[0]->SetOrderX(fOrdx);
} 


void CUIGround::SendMsg(MSG_BASE& pMsg)
{
	g_CSM.SendMsg(pMsg);
}
void CUIGround::SetRoomID(int nRoomID)
{
	m_PlayerManager->GetMainPlayer()->SetRoomID(nRoomID);
}


void CUIGround::Init()
{
	for(int i = 0 ;i < nMaxBirdCount ; ++i)
	{
		m_vecBird[i]->SetLocation(nFirstBirdX,nBirdDisY*i+nFirstBirdY);
		m_vecBird[i]->SetNext(NULL);
		m_vecBird[i]->SetCloudIndex(-1);
	}
	for(int i = 0 ;i < nMaxCloudCount ; ++i)
	{
		m_vecCloud[i]->SetLocation(CloudDis*i,nCloudY);
		m_vecCloud[i]->Init();
	}	
	m_PlayerManager->GetMainPlayer()->SetGameStart(true);
}

void CUIGround::SetGameState(bool SetGameState)
{
	//m_bReady = bReady;
	m_PlayerManager->GetMainPlayer()->SetGameStart(SetGameState);

};

void CUIGround::SetWinner(int nPlayerID)
{
	if(m_PlayerManager->GetMainPlayer()->GetPlayerID() == nPlayerID)
	{
		CShowMessage::Instance().SetShowMesage("You Win Princess's Kiss",10);
	}
	else
	{
		string sName = 	m_PlayerManager->GetPlayerList()[1]->GetName();
		sName = sName + " Win Princess's Kiss";
		CShowMessage::Instance().SetShowMesage(sName,10);
	}
	m_PlayerManager->GetMainPlayer()->SetWinGame()	;

}