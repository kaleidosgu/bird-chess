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

//template<class Obj,typename MemFun>
//callback_base* make_callback(Obj,MemFun mmeFun)
//{
//	callback_base* pCallBack = new Call
//}
//
//#define SET_CALLBACK_FUNC(func,newfunc) \
//	set_callback_##func(newfunc)\
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
	virtual ~NONOEditText();
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




class NONOUIText : public hgeGUIObject
{
public:
	NONOUIText(int id, float x, float y, float w, float h, hgeFont *fnt);

	void			SetMode(int _align);
	void			SetText(const char *_text);
	const char*		GetText(){return text;};
	void			printf(const char *format, ...);

	virtual void	Render();
	virtual void Render(float m_x,float m_y);


private:
	hgeFont*		font;
	float			tx, ty;
	int				align;
	char			text[256];
};




class NONOhgeGUIText : public NONOUIText
{
public:
	NONOhgeGUIText(int id, float x, float y, float w, float h, hgeFont *fnt);
	virtual bool	KeyClick(int key, int chr);
	virtual void 	Render();
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
	}  
	 void  SetMaxTextLen(int nLen){m_nMaxText = nLen;};
	 std::string& GetString() { return m_strbuf;};
protected:
	std::string m_strbuf;
	bool is_mouse_over_;
	float mx_,my_; 	
	int m_nMaxText;
	//pBackFun p_backFun;
};

class NONOhgePassWordGUIText:public NONOhgeGUIText
{
public:
	NONOhgePassWordGUIText(int id, float x, float y, float w, float h, hgeFont *fnt);
	virtual bool	KeyClick(int key, int chr);
private:
	std::string  m_strbufShow;
};


////////////////////////���������Ŀؼ�//////////////
#include ".\cn\GfxFont.h"

template < typename _Fnt >
class NONOGFXUIText : public hgeGUIObject
{
public:

	NONOGFXUIText(int _id, float x, float y, float w, float h, _Fnt* fnt)
	{
		id=_id;
		bStatic=true;
		bVisible=true;
		bEnabled=true;
		rect.Set(x, y, x+w, y+h);

		font=fnt;
		tx=x;
		ty=y+(h-fnt->GetHeight())/2.0f;

		text[0]=0;
	}
	void			SetMode(int _align)
	{
		align=_align;
		if(align==HGETEXT_RIGHT) tx=rect.x2;
		else if(align==HGETEXT_CENTER) tx=(rect.x1+rect.x2)/2.0f;
		else tx=rect.x1;
	}
	void			SetText(const char *_text)
	{
		strcpy_s(text, _text);
	}
	const char*		GetText(){return text;}
	//void			printf(const char *format, ...);
	virtual void	Render()
	{
		font->SetColor(color);
		font->Render(tx,ty,align,text);
	}
	virtual void Render(float m_x,float m_y)
	{
		font->SetColor(color);
		font->Render(m_x,m_y,align,text);
	}


private:
	_Fnt*			font;
	float			tx, ty;
	int				align;
	char			text[256];
};



#endif  