#include "WndStatic.h"
#include <hgefont.h>
#include <string>
CWndStatic::CWndStatic(void)
:m_strText(""),m_nAlign(0),m_crColor(0)
{
}

CWndStatic::~CWndStatic(void)
{
}

void CWndStatic::OnDraw()
{
	CWndBase::OnDraw();
 	m_pFont->SetColor(m_crColor);
 	m_pFont->Render(m_rcAbsWnd.left,m_rcAbsWnd.top,m_nAlign,m_strText.c_str());
}

bool CWndStatic::Create( int x, int y, int cx, int cy, const char* pText, CWndBase* pParent, int nID, hgeFont* pFont, int nAligh )
{
	bool bRes = CWndBase::Create( x, y, cx, cy, pParent, nID);
	m_nAlign = nAligh;
	if(m_nAlign == HGETEXT_RIGHT) 
	{
		m_rcRect.left = m_rcRect.left + m_size.cx;
	}
	else if( m_nAlign == HGETEXT_LEFT )
	{
		m_rcRect.left = m_rcRect.left;
	}
	else
	{
		m_rcRect.left = (m_rcRect.left + m_size.cx + m_size.cx ) / 2.0f;
	}
	m_crColor = 0xFFFFFFFF;
	m_pFont = pFont;
	ty = 0;//y + ( cy - m_pFont->GetHeight() ) / 2.0f;
	SetText( pText );
	MoveWindow(m_rcRect.left,m_rcRect.top);
	return bRes;
}

void CWndStatic::SetText( const char* pText )
{
	m_strText = pText;
}

void CWndStatic::SetFontColor( DWORD drColor )
{
	m_crColor = drColor;
}

const char* CWndStatic::GetText() const
{
	return m_strText.c_str();
}

bool CWndStatic::CreateNoFont( int x, int y, int cx, int cy, const char* pText, CWndBase* pParent, int nID )
{
	m_pFont = m_pHgeFont;
	return CWndStatic::Create(x, y, cx, cy, pText,pParent,nID, m_pFont, HGETEXT_LEFT);
}

float CWndStatic::GetStringWidth( const char* pChar, bool bMulti )
{
	float fNum = 0.0f;
	fNum = m_pHgeFont->GetStringWidth( pChar, true );
	return fNum;
}
void CWndStatic::AddText( const char* pText )
{
	m_strText = m_strText + "\n" + pText ;
}