#include "UILoginGame.h"
#include "../cardgame/cardgamemsg.h"
#include "../netclient/ClientSocketMgr.h"
#include "NonoHGECtrlEx.h"

using namespace std;
extern HGE *hge;
extern clinetnet::CClientSocketMgr g_CSM;
extern string G_IPString;
extern int G_nPort;


CUILoginGame::CUILoginGame(int x,int y,hgeFont*fnt):m_x(x),m_y(y),m_bEnable(true)
{
	//m_bEnable = false;
	_CreateBG(x,y);
	_CreateEdit(x,y,fnt);
	//m_AccountName->SetBackFunc(this->_OnEnter);
}

CUILoginGame::~CUILoginGame()
{
	delete m_LogUI;
	delete m_bg;
}

void CUILoginGame::SetEnable(bool bEnable)
{
	m_bEnable = bEnable;
}


void CUILoginGame::SendLog2Server(const string& name,const string& password)
{
	g_CSM.Connect(G_IPString,G_nPort);
	MSG_CARDGAME_C2S_LoginRequest msg;
	strcpy_s(msg.szUsername,cMAX_USERNAME_LEN,name.c_str());
	strcpy_s(msg.szPassword,cMAX_PASSWORD_LEN,password.c_str());
	g_CSM.SendMsg(msg);
}

void CUILoginGame::Update(float dt)
{
	if(m_bEnable)
	{
		if(hge->Input_GetKey() == HGEK_ENTER || hge->Input_GetKey() == HGEK_TAB)
		{
			_OnEnter(m_LogUI->GetFocus());
		}
		else
			m_LogUI->Update(dt);
	}
}

void CUILoginGame::Render()
{
	if(m_bEnable)
	{
		m_bg->Render(float(m_x),float(m_y));
		m_LogUI->Render();
	}
}
//enum{EIDT_ACCOUNTNAME  = 1 ,EDIT_PASSWORD}

void CUILoginGame::_CreateEdit(int x,int y,hgeFont*fnt)
{
	m_LogUI =new hgeGUI();
	m_AccountName = new NONOhgeGUIText(EIDT_ACCOUNTNAME,float(x+20),float(y+180),float(97),float(70),fnt);
	m_AccountName->SetMode(HGETEXT_LEFT);
	//m_AccountName->SetColor(0xFFD4C25A);
	m_AccountName->SetMaxTextLen(cMAX_USERNAME_LEN);
	m_LogUI->AddCtrl(m_AccountName);
	m_Password = new NONOhgePassWordGUIText(EDIT_PASSWORD,float(x+20+97),float(y+180),float(97),float(70),fnt);
	m_Password->SetMode(HGETEXT_LEFT);
	m_Password->SetColor(0xFFD4C25A);
	m_Password->SetMaxTextLen(cMAX_PASSWORD_LEN);
	m_LogUI->AddCtrl(m_Password);
	m_AccountName->MouseLButton(true);
}

void CUILoginGame::_CreateBG(int x,int y)
{
	if(hge)
	{
		HTEXTURE tex=hge->Texture_Load("res/Pic/UILoginBG.png");
		m_bg= new hgeSprite(tex,459,4,233,239);
	}
}


void CUILoginGame::_OnEnter(int nCtlID)
{
	 	if(nCtlID == EIDT_ACCOUNTNAME)
	 	{
	 		m_Password->MouseLButton(true);
	 	}
	 	else if(nCtlID == EDIT_PASSWORD)
	 	{
	 		if(m_AccountName->GetString().empty())
	 		{
	 			m_AccountName->MouseLButton(true);
	 		}
	 		else if(m_AccountName->GetString().empty())
	 		{
	 			m_AccountName->MouseLButton(true);
	 		}
	 		else
	 		{
				//这里以后去除。先加到服务器上去。
	 			SendLog2Server(m_AccountName->GetString(),m_Password->GetString());
	 		}
	 	}
}
