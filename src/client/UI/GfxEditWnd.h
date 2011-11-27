#pragma once
#include <WndBase.h>
class GfxEdit;
class CGfxEditWnd :
	public CWndBase
{
public:
	CGfxEditWnd(void);
	virtual ~CGfxEditWnd(void);
	virtual bool Create( int x, int y, int cx, int cy, CWndBase* pParent, int nID );
	void SetTextFont( UINT nEditWidth, DWORD nFontColor, char* lpFontName = "ËÎÌו", UINT nFontSize = 12, BOOL bBold = FALSE, BOOL bItalic = FALSE, BOOL bAntialias = TRUE );
	void SetText( const char* pText );
	void SetFontColor( DWORD drColor );
	virtual void OnDraw();
	void AddText( const char* pText );
	virtual int OnLButtonDown(const UINT &nFlags,const POINT &point);
	void DrawCaret();
private:
	GfxEdit* m_pGfxEdit;
	std::string m_strText;
};
