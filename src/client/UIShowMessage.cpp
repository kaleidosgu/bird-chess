/************************************************************************/
/*					简单提示单件,showmessage用,在界面显示3秒后消失        */
/************************************************************************/
#include "NonoHGECtrlEx.h"
#include "UIShowMessage.h"

extern hgeFont* fnt_WinOrLost;
CShowMessage::CShowMessage():m_bVisable(false),m_fTime(2.0f)
{
	m_ShowMesage = new NONOUIText(1,640,300,20,10,fnt_WinOrLost);
	m_ShowMesage->SetMode(HGETEXT_CENTER);
}

CShowMessage::~CShowMessage()
{
}

CShowMessage& CShowMessage::Instance()
{
	static CShowMessage Message;
	return Message;
}


void CShowMessage::Render()
{
	if (m_bVisable)
	{
		m_ShowMesage->Render();
	}
}


void CShowMessage::Update(float dt)
{
	if(m_fTime>0)
	{
		m_fTime-=dt;
		if(m_fTime<0)
		{
			m_bVisable = false;
		}
	}
	
	m_ShowMesage->Update(dt);
}

void CShowMessage::SetShowMesage(const std::string& str,float fTime)
{
	m_ShowMesage->SetText(str.c_str());
	m_fTime = fTime;
	m_bVisable = true;
}
