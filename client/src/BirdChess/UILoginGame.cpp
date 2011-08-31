#include "UILoginGame.h"
#include "NetAdd/cardgamemsg.h"
#include "NetAdd/ClientSocketMgr.h"
#include "NonoHGECtrlEx.h"

using namespace std;
extern HGE *hge;
extern clinetnet::CClientSocketMgr g_CSM;


CUILoginGame::CUILoginGame(int x,int y,hgeFont*fnt):m_x(x),m_y(y)
{
	_CreateBG(x,y);
	_CreateEdit(x,y,fnt);
	//m_AccountName->SetBackFunc(this->_OnEnter);
}

CUILoginGame::~CUILoginGame()
{
	delete m_LogUI;
	delete m_bg;
}


void CUILoginGame::SendLog2Server(const string& name,const string& password)//这个暂时放这里,可能会用到一个全局的啥啥了
{
	MSG_CARDGAME_C2S_LoginRequest msg;
	strcpy_s(msg.szUsername,cMAX_USERNAME_LEN,name.c_str());
	strcpy_s(msg.szPassword,cMAX_PASSWORD_LEN,password.c_str());
	g_CSM.SendMsg(msg);
}

void CUILoginGame::Update(float dt)
{

	if(hge->Input_GetKey() == HGEK_ENTER || hge->Input_GetKey() == HGEK_TAB)
	{
		_OnEnter(m_LogUI->GetFocus());
	}
	else
		m_LogUI->Update(dt);

}

void CUILoginGame::Render()
{
	m_bg->Render(m_x,m_y);
	m_LogUI->Render();
}
	//enum{EIDT_ACCOUNTNAME  = 1 ,EDIT_PASSWORD}

void CUILoginGame::_CreateEdit(int x,int y,hgeFont*fnt)
{
	m_LogUI =new hgeGUI();
	m_AccountName = new NONOhgeGUIText(EIDT_ACCOUNTNAME,x+20,y+212,97,70,fnt);
	m_AccountName->SetMode(HGETEXT_LEFT);
	m_AccountName->SetColor(0xFFD4C25A);
	m_LogUI->AddCtrl(m_AccountName);
	m_Password = new NONOhgeGUIText(EDIT_PASSWORD,x+20+97,y+212,97,70,fnt);
	m_Password->SetMode(HGETEXT_LEFT);
	m_Password->SetColor(0xFFD4C25A);
	m_LogUI->AddCtrl(m_Password);

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
		if(m_AccountName->m_strbuf.empty())
		{
			m_AccountName->MouseLButton(true);
		}
		else if(m_AccountName->m_strbuf.empty())
		{
			m_AccountName->MouseLButton(true);
		}
		else
		{
			SendLog2Server(m_AccountName->m_strbuf,m_Password->m_strbuf);
		}
	}
}
