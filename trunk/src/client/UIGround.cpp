#include "../netclient/ClientSocketMgr.h"
#include <sstream>
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
#include ".\cn\GfxFont.h"
#include ".\cn\GfxEdit.h"

CUIGround* g_UIGround = NULL;
const int nMaxBirdCount = 5;
const int nMaxCloudCount = 10;
const int nFirstBirdY = 180;
const int nFirstBirdX = 30;
const int nBirdDisY = 100;
const int nCloudY = 650;
const int nMaxChatTxtShow = 4;
extern clinetnet::CClientSocketMgr g_CSM;
GfxFont* G_GfxFnt;
hgeFont* G_HgeFnt;
using namespace std;

char CUIGround::m_mainPlayerName[128] = {0};
int CUIGround::m_nRoomID = 0;
bool CUIGround::m_bShowChatEdit = false;
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
	G_GfxFnt = new GfxFont("黑体",20,true,false,true);// 宋书，粗体，非斜体，非平滑
	for(int i=0 ;i < nMaxChatTxtShow;i++)
	{
		UIChatTxt* m_Chat = new UIChatTxt(1,800,900,100,100,G_GfxFnt);
		//G_GfxFnt->SetColor(0xFFFFF00F);	// 设置像素字体颜色..貌似无效
		m_vecChat.push_back(m_Chat);
	}

	m_Edit = new GfxEdit(600,0xFFFFFF00, "黑体", 20,false,false);// 编辑框字体指定 隶书
	m_Edit->SetCallback(&CUIGround::OnEnter);
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

	deque<UIChatTxt*>::iterator itBeginChat= m_vecChat.begin();
	deque<UIChatTxt*>::iterator itEndChat = m_vecChat.end();
	for(;itBeginChat!=itEndChat;++itBeginChat)
	{
		delete *itBeginChat;
	}
	m_vecChat.clear();
	delete m_PlayerManager;m_PlayerManager =0;
	delete G_GfxFnt;
	delete m_Edit;
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
		deque<UIChatTxt*>::iterator itBeginChat= m_vecChat.begin();
		deque<UIChatTxt*>::iterator itEndChat = m_vecChat.end();
		for(int i = 0;itBeginChat!=itEndChat;++itBeginChat,++i)
		{
			(*itBeginChat)->Render(900,648+20*i);
		}

		if(m_bShowChatEdit)
			m_Edit->Render(900,738);
	}
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

		deque<UIChatTxt*>::iterator itBeginChat= m_vecChat.begin();
		deque<UIChatTxt*>::iterator itEndChat = m_vecChat.end();
		for(;itBeginChat!=itEndChat;++itBeginChat)
		{
			(*itBeginChat)->Update(dt);
		}
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
	m_Edit->SetFocus();
	m_PlayerManager->GetMainPlayer()->SetPlayerInfo(rPlayerInfo.szPlayerName,rPlayerInfo.nPlayerID);
	sprintf_s(m_mainPlayerName,"[%s]:",rPlayerInfo.szPlayerName);
	m_Edit->InsertCookie(m_mainPlayerName);
	string str = rPlayerInfo.szPlayerName;
	stringstream ss;
	ss << "[战绩]"<<str << "("<<rPlayerInfo.nScore<<")"<<rPlayerInfo.nWin <<"/"<< rPlayerInfo.nWin <<"/" << rPlayerInfo.nWin;
	ShowChat(ss.str());
	ShowChat("[聊天]");

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
		string str = rPlayerInfo.szPlayerName;
		stringstream ss;
		int nRoom(m_nRoomID);
		ss << str << "("<<rPlayerInfo.nScore<<")"<<rPlayerInfo.nWin <<"/"<< rPlayerInfo.nWin <<"/" << rPlayerInfo.nWin <<""<< "进入房间";
		ShowChat(ss.str());
	}
}

void CUIGround::SetPlayerCardInfo(int nPosID,int nType,int nInstruction)
{
	 m_PlayerManager->GetMainPlayer()->SetPlayerCardInfo(nPosID,nType,nInstruction);
}


void CUIGround::SetPlayerBirdID(int nPlayerCardType,bool MainPlayer )
{
	if(MainPlayer)
	{
		m_PlayerManager->GetMainPlayer()->SetBirdID(nPlayerCardType);
	}
	else
	{
		std::vector<UIPlayer*>&  rPlayerVec =  m_PlayerManager->GetPlayerList();
		rPlayerVec[1]->SetBirdID(nPlayerCardType);
	}
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
	m_nRoomID = nRoomID;
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
	std::vector<UIPlayer*>&  rPlayerVec =  m_PlayerManager->GetPlayerList();
	rPlayerVec[1]->SetBirdID(0);
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

void CUIGround::SendChat( const std::string& strChat,int nType /*= 0*/ )
{
	MSG_CARDGAME_X2X_ChatContent* msg;
	int nSize =  strChat.size()+1;
	msg = CreateDynamicLengthMsg(nSize+sizeof(MSG_CARDGAME_X2X_ChatContent),(MSG_CARDGAME_X2X_ChatContent*)0);
	strcpy_s(msg->pData,nSize,strChat.c_str());
	SendMsg(*msg);
}

void CUIGround::ShowChat(const  std::string& strChat,int nType /*= 0*/ )
{
	deque<UIChatTxt*>::iterator itBeginChat= m_vecChat.begin();
	deque<UIChatTxt*>::iterator itEndChat = m_vecChat.end();
	for(int i = 0;itBeginChat!=itEndChat;++itBeginChat,++i)
	{
		if(i!=nMaxChatTxtShow-1)
			(*itBeginChat)->SetText(m_vecChat[i+1]->GetText());
		else
			(*itBeginChat)->SetText(strChat.c_str());
	}
	
}


void CUIGround::OnEnter( GfxEdit* m_Edit )
{
	if(m_bShowChatEdit)
	{
		const char* cSend = m_Edit->GetCookie();
		std::string strSend(cSend);
		std::string str2Send(m_mainPlayerName);
		CmdProcess(cSend);
		if (strSend.length() > str2Send.length())
		{
			CUIGround::SendChat(cSend);
		}
		else
		{
			CUIGround::SendChat(m_mainPlayerName);
		}
	}
	m_Edit->ClearCookie();
	m_Edit->InsertCookie(m_mainPlayerName);
}

void CUIGround::CmdProcess( const std::string& strCmd )
{
	//string str2 = str.substr(str2.find('\\'),5);
	int cmd= strCmd.find("/room");
	if(cmd != -1)
	{
		string str2 = strCmd.substr(cmd+5,strCmd.size());

		stringstream ss;
		int nRoom(m_nRoomID);
		ss << str2;// int->string
		ss>>nRoom;
		if(nRoom!=m_nRoomID)
		{
			MSG_CARDGAME_C2S_EnterRoom room;
			room.nRoomID = nRoom;
			SendMsg(room);
		}
	}
	//return false;
}

void CUIGround::BirdChat( const std::string& strChat,int nType /*= 0*/ )
{
	std::string str2Send(m_mainPlayerName);
	str2Send += strChat;
	CmdProcess(strChat);
	CUIGround::SendChat(str2Send);
}