//////////////////////////////////////////////////////////////////////////
//
//	扩展的控件 
//	NONOhgeGUIText 登录输入密码用 还没有焦点亮显示和光标....凑合用吧,如果有聊天也能用这个
//	争取把中文显示也做出来
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
///****** 默认带中文字体的基类 ******/  
//class GDE_BASIC_GUIChineseFont : public hgeGUIObject  
//{  
//public:  
//	GDE_BASIC_GUIChineseFont()  
//	{  
//		font = new GfxFont("宋体",20,TRUE,FALSE,FALSE);// 宋体，粗体，非斜体，非平滑  
//		font->SetColor(0xFFFFFFFF);      // 设置像素字体颜色,默认白色  
//	}  
//
//	//fonttype 字体,size 文字大小  
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
//	//以某种颜色显示一次，然后还原颜色  
//	void FontPrintOnce( int x,int y, DWORD Color, std::string txt )  
//	{  
//		DWORD oldcolor = font->GetColor();  
//		font->SetColor( Color );  
//		font->Print( x,y, txt.c_str() );  
//		font->SetColor( oldcolor );  
//	}  
//
//protected:  
//	GfxFont* font;//中文字体指针  
//};  

/****** 只用于显示，不用于控制的基类 ******/  
class GDE_BASIC_GUIViewOnly : public hgeGUIObject  
{  
public:  
	GDE_BASIC_GUIViewOnly()  
	{  
		this->rect.Set( 0,0,0,0 );//这样不会覆盖其他GUI控件的控制区  
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
	float mx_,my_; //当前鼠标位置  
	bool is_mouse_over_; //鼠标是否悬停  
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
	float mx_,my_; //鼠标位置  
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