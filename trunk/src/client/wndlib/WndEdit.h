#pragma once
#include "wndbase.h"
class hgeFont;
class CWndStatic;
class CWndEdit :
	public CWndBase
{
public:
	CWndEdit(void);
	virtual ~CWndEdit(void);
	virtual bool Create( int x, int y, int cx, int cy, const char* pText, CWndBase* pParent, int nID, hgeFont* pFont, int nAligh );
	virtual bool CreateNoFont( int x, int y, int cx, int cy, CWndBase* pParent, int nID );
	virtual int OnChar( const hgeInputEvent& rEvent );
	virtual void OnDraw();
	virtual void OnUpdate( float ft );
	void SetText( const char* pText );
	float GetStringWidth( const char* pChar, bool bMulti );
	void SetNotifyParent( bool bNotify );
	const char* GetText() const;
	void SetMask( const char* pMask );
	virtual int OnLButtonDown(const UINT &nFlags,const POINT &point);
protected:
	void DrawCaret();
	virtual void OnSetFocus();
private:
	int			m_nCaretPos;
	CWndStatic* m_pStatic;
	bool		m_bDrawCaret;

	RECT		m_rcCaret;
	float		m_fCurTime;
	bool		m_bNotifyParent;
	std::string m_strText;
	std::string m_strShowText;
	std::string m_strMask;
};
