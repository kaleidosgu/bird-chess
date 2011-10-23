#pragma once
#include "wndbase.h"
class hgeFont;
class CWndStatic :
	public CWndBase
{
public:
	CWndStatic(void);
	virtual ~CWndStatic(void);
	virtual void OnDraw();
	virtual bool Create( int x, int y, int cx, int cy, const char* pText, CWndBase* pParent, int nID, hgeFont* pFont, int nAligh );
	virtual bool CreateNoFont( int x, int y, int cx, int cy, const char* pText, CWndBase* pParent, int nID );
	void SetText( const char* pText );
	void SetFontColor( DWORD drColor );
	const char* GetText() const;
	void AddText( const char* pText );

	float GetStringWidth( const char* pChar, bool bMulti );
	void SetScale(float scale);
private:
	hgeFont*		m_pFont;
	std::string		m_strText;
	int				m_nAlign;
	DWORD			m_crColor;
	int				tx;
	int				ty;
};
