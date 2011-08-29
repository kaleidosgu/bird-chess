#include "NonoHGECtrlEx.h"


NONOEditText::NONOEditText(int id, float x, float y, float w, float h, hgeFont *fnt)
{  
	m_Text = new hgeGUIText(id,x,y,w,h,fnt);
}  
//
//NONOEditText::~NONOEditText()
//{
//
//}

void NONOEditText::printf(const char *format,...)
{
	//m_Text->printf(format,...);
// 	char			text[256];
// 	vsprintf(text, format, (char *)&format+sizeof(format));
	m_Text->SetText("123");
};

void NONOEditText::Render()
{
	m_Text->Render();	
}

bool NONOEditText::KeyClick(int key, int chr) 
{
	switch (key)
	{
		case HGEK_BACKSPACE:
			m_strbuf.erase();
			m_Text->SetText(m_strbuf.c_str());
			break;

		case HGEK_0:
		case HGEK_1:
		case HGEK_2:
		case HGEK_3:
		case HGEK_4:
		case HGEK_5:
		case HGEK_6:
		case HGEK_7:
		case HGEK_8:
		case HGEK_9:
		case HGEK_A:
		case HGEK_B:
		case HGEK_C:
		case HGEK_D:
		case HGEK_E:
		case HGEK_F:
		case HGEK_G:
		case HGEK_H:
		case HGEK_I:
		case HGEK_J:
		case HGEK_K:
		case HGEK_L:
		case HGEK_M:
		case HGEK_N:
		case HGEK_O:
		case HGEK_P:
		case HGEK_Q:
		case HGEK_R:
		case HGEK_S:
		case HGEK_T:
		case HGEK_U:
		case HGEK_V:
		case HGEK_W:
		case HGEK_X:
		case HGEK_Y:
		case HGEK_Z:
			m_strbuf.push_back(char(HGEK_Y));
			m_Text->SetText(m_strbuf.c_str());
			break;
	}
	return false;
}

 

NONOhgeGUIText::NONOhgeGUIText(int id, float x, float y, float w, float h, hgeFont *fnt)
:hgeGUIText(id,x,y,w,h,fnt)
{
	bStatic = false;
}

bool NONOhgeGUIText::KeyClick(int key, int chr) 
{
	switch (key)
	{
	case HGEK_BACKSPACE:
		m_strbuf = m_strbuf.substr(0, m_strbuf.length()-1);
		SetText(m_strbuf.c_str());
		break;
	case HGEK_ENTER:
		if(p_backFun)
			//p_backFun(id);
		break;
	case HGEK_0:
	case HGEK_1:
	case HGEK_2:
	case HGEK_3:
	case HGEK_4:
	case HGEK_5:
	case HGEK_6:
	case HGEK_7:
	case HGEK_8:
	case HGEK_9:
	case HGEK_A:
	case HGEK_B:
	case HGEK_C:
	case HGEK_D:
	case HGEK_E:
	case HGEK_F:
	case HGEK_G:
	case HGEK_H:
	case HGEK_I:
	case HGEK_J:
	case HGEK_K:
	case HGEK_L:
	case HGEK_M:
	case HGEK_N:
	case HGEK_O:
	case HGEK_P:
	case HGEK_Q:
	case HGEK_R:
	case HGEK_S:
	case HGEK_T:
	case HGEK_U:
	case HGEK_V:
	case HGEK_W:
	case HGEK_X:
	case HGEK_Y:
	case HGEK_Z:
		m_strbuf.push_back(char(key));
		SetText(m_strbuf.c_str());
		break;
	}
	return false;
}
