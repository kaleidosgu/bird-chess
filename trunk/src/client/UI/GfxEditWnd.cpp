#include "GfxEditWnd.h"
#include "../../third/wndlib/src/wnddatadefine.h"
#include "../cn/GfxEdit.h"
#include <string>
CGfxEditWnd::CGfxEditWnd(void)
:m_pGfxEdit(NULL),m_strText("")
{
}

CGfxEditWnd::~CGfxEditWnd(void)
{
}

bool CGfxEditWnd::Create( int x, int y, int cx, int cy, CWndBase* pParent, int nID )
{
	return CWndBase::Create( x, y, cx, cy, pParent, nID );
}


void CGfxEditWnd::SetTextFont( UINT nEditWidth, DWORD nFontColor, char* lpFontName /*= "ËÎÌו"*/, UINT nFontSize /*= 12*/, BOOL bBold /*= FALSE*/, BOOL bItalic /*= FALSE*/, BOOL bAntialias /*= TRUE */ )
{
	if( m_pGfxEdit )
	{
		delete m_pGfxEdit;
	}
	m_pGfxEdit		= new GfxEdit( nEditWidth, nFontColor, lpFontName, nFontSize, bBold, bItalic, bAntialias );
}
void CGfxEditWnd::SetText( const char* pText )
{
	m_strText = pText;
}

void CGfxEditWnd::SetFontColor( DWORD drColor )
{
	if( m_pGfxEdit )
	{
		//m_pGfxEdit->SetColor( drColor );
	}
}

void CGfxEditWnd::OnDraw()
{
	if (!IsVisible())
	{
		return;
	}
	CWndBase::OnDraw();
	if( m_pGfxEdit )
	{
		//m_pGfxEdit->Print( (float)m_rcAbsWnd.left,(float)m_rcAbsWnd.top,0,m_strText.c_str());
		m_pGfxEdit->Render( (float)m_rcAbsWnd.left,(float)m_rcAbsWnd.top );
	}

	DrawCaret();
}

void CGfxEditWnd::AddText( const char* pText )
{
	m_strText = m_strText + "\n" + pText ;
}

int CGfxEditWnd::OnLButtonDown( const UINT &nFlags,const POINT &point )
{
	if(PtInWnd(point))
	{
		SetFocus(this);
		m_pGfxEdit->SetFocus();
		return WND_RESULT_YES;
	}
	return WND_RESULT_NO;
}

void CGfxEditWnd::DrawCaret()
{
	if ( m_pWndFocus == this )
	{		
		if( m_pDevice )
		{
			//SColor rc(255,0,0,0);
// 			m_pDevice->hge->Gfx_RenderLine((float)(m_nCaretPos + m_rcAbsWnd.left),(float)(m_rcAbsWnd.top + 5),(float)(m_nCaretPos + m_rcAbsWnd.left) ,(float)(m_rcAbsWnd.top + 18),m_crCaret);
// 			m_pDevice->hge->Gfx_RenderLine((float)(m_nCaretPos + m_rcAbsWnd.left + 1),(float)(m_rcAbsWnd.top + 5),(float)(m_nCaretPos + m_rcAbsWnd.left + 1),(float)(m_rcAbsWnd.top + 18),m_crCaret);
// 			m_pDevice->hge->Gfx_RenderLine((float)(m_nCaretPos + m_rcAbsWnd.left + 2),(float)(m_rcAbsWnd.top + 5),(float)(m_nCaretPos + m_rcAbsWnd.left + 2),(float)(m_rcAbsWnd.top + 18),m_crCaret);
		}
	}
}