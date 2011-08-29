//////////////////////////////////////////////////////////////////////////
//
//	��չ�Ŀؼ� 
//	NONOhgeGUIText ��¼���������� ��û�н�������ʾ�͹��....�պ��ð�,���������Ҳ�������
//	��ȡ��������ʾҲ������
//
//
//////////////////////////////////////////////////////////////////////////
#ifndef GDE_UI_BasicClasses_H_  
#define GDE_UI_BasicClasses_H_  

#include "hge.h"  
#include "hgesprite.h"  
#include "hgefont.h"  
#include "hgerect.h"  
#include "hgegui.h"  
#include "hgeguictrls.h"  
//

#include <string>  
#include <string.h>  
#include <stdio.h>  
#include <stdlib.h>  

//
///****** Ĭ�ϴ���������Ļ��� ******/  
//class GDE_BASIC_GUIChineseFont : public hgeGUIObject  
//{  
//public:  
//	GDE_BASIC_GUIChineseFont()  
//	{  
//		font = new GfxFont("����",20,TRUE,FALSE,FALSE);// ���壬���壬��б�壬��ƽ��  
//		font->SetColor(0xFFFFFFFF);      // ��������������ɫ,Ĭ�ϰ�ɫ  
//	}  
//
//	//fonttype ����,size ���ִ�С  
//	GDE_BASIC_GUIChineseFont( std::string fonttype , int size )  
//	{  
//		font = new GfxFont(fonttype.c_str(),size,TRUE,FALSE,FALSE);  
//		font->SetColor(0xFFFFFFFF);        
//	}  
//
//	~GDE_BASIC_GUIChineseFont()  
//	{  
//		if( font )   
//			delete font;  
//	}  
//
//	//��ĳ����ɫ��ʾһ�Σ�Ȼ��ԭ��ɫ  
//	void FontPrintOnce( int x,int y, DWORD Color, std::string txt )  
//	{  
//		DWORD oldcolor = font->GetColor();  
//		font->SetColor( Color );  
//		font->Print( x,y, txt.c_str() );  
//		font->SetColor( oldcolor );  
//	}  
//
//protected:  
//	GfxFont* font;//��������ָ��  
//};  

/****** ֻ������ʾ�������ڿ��ƵĻ��� ******/  
class GDE_BASIC_GUIViewOnly : public hgeGUIObject  
{  
public:  
	GDE_BASIC_GUIViewOnly()  
	{  
		this->rect.Set( 0,0,0,0 );//�������Ḳ������GUI�ؼ��Ŀ�����  
	}  
};  


class GDE_BASIC_GUIMouseSensitive : public hgeGUIObject  
{  
public:  
	GDE_BASIC_GUIMouseSensitive()  
		: mx_ ( 0 )  
		, my_ ( 0 )  
		, is_mouse_over_ ( FALSE )  
	{  
	}  

	virtual ~GDE_BASIC_GUIMouseSensitive(){}  

	virtual void MouseOver( bool bOver )  
	{  
		is_mouse_over_ = bOver;  
	}  

	virtual bool MouseMove(float x, float y)  
	{  
		mx_ = x;  
		my_ = y;  
		return false;  
	}  

protected:  
	float mx_,my_; //��ǰ���λ��  
	bool is_mouse_over_; //����Ƿ���ͣ  
};  

class NONOEditText : public hgeGUIObject  
{  
public:  
	//NONOEditText();
	//virtual ~NONOEditText();  
	NONOEditText(int id, float x, float y, float w, float h, hgeFont *fnt);
	virtual void MouseOver( bool bOver )  
	{  
		is_mouse_over_ = bOver;  
		Focus(true);
	}  

	virtual bool MouseMove(float x, float y)  
	{  
		mx_ = x;  
		my_ = y;  
		return false;  
	}  
	
	virtual void	Render();

	//virtual void	Update(float dt) {}

	//virtual void	Enter() {}
	//virtual void	Leave() {}
	//virtual void	Reset() {}
	//virtual bool	IsDone() { return true; }
	//virtual void	Focus(bool bFocused) {}
	//virtual void	MouseOver(bool bOver) {}

	//virtual bool	MouseMove(float x, float y) { return false; }
	virtual bool	MouseLButton(bool bDown) { Focus(true);return true; }
	virtual bool	MouseRButton(bool bDown) { return false; }
	virtual bool	KeyClick(int key, int chr);
	virtual void	SetColor(DWORD _color) { color=_color; }

	void			SetMode(int _align){m_Text->SetMode(_align);};
	void			SetText(const char *_text){m_Text->SetText(_text);};
	void			printf(const char *format,...);

protected:  
	float mx_,my_; //���λ��  
	bool is_mouse_over_; 
	hgeGUIText* m_Text;
	std::string m_strbuf;
};  

typedef void(*pBackFun)(int);

class NONOhgeGUIText : public hgeGUIText
{
public:
	NONOhgeGUIText(int id, float x, float y, float w, float h, hgeFont *fnt);
	virtual bool	KeyClick(int key, int chr);
	virtual bool	MouseLButton(bool bDown)
	{
		Focus(true);
		gui->SetFocus(id);
		return true;
	};
	virtual bool    MouseMove(float x, float y)  
	{  
		mx_ = x;  
		my_ = y;  
		return false;  
	}  
	virtual void MouseOver( bool bOver )  
	{  
		is_mouse_over_ = bOver;
		//Focus(true);
		//gui->SetFocus(id);
	}  
	virtual void SetBackFunc(pBackFun pFun)
	{
		p_backFun = pFun;
	}
	bool is_mouse_over_;
	float mx_,my_; 
	std::string m_strbuf;
	pBackFun p_backFun;
};



#endif  