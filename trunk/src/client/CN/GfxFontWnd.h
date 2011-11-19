#pragma once
#include <WndBase.h>
class GfxFont;
class CGfxFontWnd :
	public CWndBase
{
public:
	CGfxFontWnd(void);
	virtual ~CGfxFontWnd(void);
	virtual bool Create( int x, int y, int cx, int cy, CWndBase* pParent, int nID );
	void SetTextFont( const char* lpsFontName, int nFaceSize, BOOL bBold = FALSE, BOOL bItalic = FALSE, BOOL bAntialias = TRUE );
	void SetText( const char* pText );
	void SetFontColor( DWORD drColor );
	virtual void OnDraw();
	void AddText( const char* pText );
private:
	GfxFont* m_pGfxFont;
	std::string m_strText;
};
