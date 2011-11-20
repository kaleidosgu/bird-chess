#include "GfxFontWnd.h"
#include "GfxFont.h"
#include <string>
CGfxFontWnd::CGfxFontWnd(void)
:m_pGfxFont(NULL),m_strText("")
{
}

CGfxFontWnd::~CGfxFontWnd(void)
{
}

bool CGfxFontWnd::Create( int x, int y, int cx, int cy, CWndBase* pParent, int nID )
{
	return CWndBase::Create( x, y, cx, cy, pParent, nID );
}

void CGfxFontWnd::SetTextFont( const char* lpsFontName, int nFaceSize, BOOL bBold /*= FALSE*/, BOOL bItalic /*= FALSE*/, BOOL bAntialias /*= TRUE */ )
{
	if( m_pGfxFont )
	{
		delete m_pGfxFont;
	}
	m_pGfxFont		= new GfxFont(lpsFontName,nFaceSize,bBold,bItalic,bAntialias);// 宋书，非粗体，非斜体，非平滑.
}

void CGfxFontWnd::SetText( const char* pText )
{
	m_strText = pText;
}

void CGfxFontWnd::SetFontColor( DWORD drColor )
{
	if( m_pGfxFont )
	{
		m_pGfxFont->SetColor( drColor );
	}
}

void CGfxFontWnd::OnDraw()
{
	if (!IsVisible())
	{
		return;
	}
	CWndBase::OnDraw();
	if( m_pGfxFont )
	{
		m_pGfxFont->Print( (float)m_rcAbsWnd.left,(float)m_rcAbsWnd.top,0,m_strText.c_str());
	}
}

void CGfxFontWnd::AddText( const char* pText )
{
	m_strText = m_strText + "\n" + pText ;
}