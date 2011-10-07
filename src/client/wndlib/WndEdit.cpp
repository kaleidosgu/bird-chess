#include "WndEdit.h"
#include "WndStatic.h"
#include "wnddatadefine.h"
#include <string>
#include "../GraphicDefine/datadefine.h"
#include "../hgedevice/HGEDevice.h"
#include <hgefont.h>
CWndEdit::CWndEdit(void):m_pStatic(NULL),m_bDrawCaret(false),m_nCaretPos(0.0f),m_bNotifyParent(false)
{
	m_fCurTime = 0.f;
	SetRectEmpty(&m_rcCaret);
	m_strText = "";
	m_strMask = "";
	m_strShowText = "";
}

CWndEdit::~CWndEdit(void)
{
}

bool CWndEdit::Create( int x, int y, int cx, int cy, const char* pText, CWndBase* pParent, int nID, hgeFont* pFont, int nAligh )
{
	bool bRes = CWndBase::Create( x, y, cx, cy, pParent, nID );
	m_pStatic = new CWndStatic;
	m_pStatic->Create(0,0,cx,cy,pText,this,0,pFont,nAligh);
	return bRes;
}

int CWndEdit::OnChar( const hgeInputEvent& rEvent )
{
	if( m_pWndFocus != this )
	{
		return WND_RESULT_NO;
	}
	if( rEvent.key == HGEK_BACKSPACE )
	{
		std::string tempStr = m_strText;
		const char* pChar = tempStr.c_str();
		if( pChar )
		{
			const size_t nLen = strlen( pChar );
			if( nLen > 0 )
			{
				tempStr.resize( nLen - 1);
				m_strText = tempStr;
				SetText( m_strText.c_str() );
			}
		}
	}
	else if( rEvent.key == HGEK_ENTER && m_bNotifyParent )
	{
		return WND_RESULT_NO;
	}
	else
	{
		char tempChar[2] = {0};
		tempChar[0] = (char)(rEvent.chr);
		std::string strText = tempChar;
		std::string strOri = m_strText;
		strText = strOri + strText;
		m_strText = strText;
		//m_pStatic->SetText( m_strText.c_str() );
		SetText(m_strText.c_str());
	}
// 	//return CWndBase::OnChar( rEvent );
	m_nCaretPos = (int)m_pStatic->GetStringWidth( m_pStatic->GetText(), true );
	return WND_RESULT_YES;
}

void CWndEdit::OnDraw()
{
	CWndBase::OnDraw();
	DrawCaret();
}

void CWndEdit::OnUpdate( float ft )
{
	m_fCurTime += ft;
	if( m_fCurTime > 0.5f )
	{
		m_bDrawCaret = !m_bDrawCaret;
		m_fCurTime = 0.0f;
	}
	CWndBase::OnUpdate( ft );
}

void CWndEdit::DrawCaret()
{
	if ( m_pWndFocus == this && m_bDrawCaret )
	{		
		if( m_pDevice )
		{
			//SColor rc(255,0,0,0);
			m_pDevice->hge->Gfx_RenderLine(m_nCaretPos + m_rcAbsWnd.left,m_rcAbsWnd.top + 5,m_nCaretPos + m_rcAbsWnd.left ,m_rcAbsWnd.top + 18,0xFFFFFFFF);
			m_pDevice->hge->Gfx_RenderLine(m_nCaretPos + m_rcAbsWnd.left + 1,m_rcAbsWnd.top + 5,m_nCaretPos + m_rcAbsWnd.left + 1,m_rcAbsWnd.top + 18,0xFFFFFFFF);
			m_pDevice->hge->Gfx_RenderLine(m_nCaretPos + m_rcAbsWnd.left + 2,m_rcAbsWnd.top + 5,m_nCaretPos + m_rcAbsWnd.left + 2,m_rcAbsWnd.top + 18,0xFFFFFFFF);
		}
	}
}

void CWndEdit::SetText( const char* pText )
{
	m_strText = pText;
	m_strShowText = pText;
	size_t szCount = m_strShowText.size();
	if( m_strMask.size() > 0 )
	{
		std::string temp = "";
		for( size_t idx = 0; idx < szCount; idx++ )
		{
			temp = temp + m_strMask;
		}
		m_strShowText = temp;
	}
	m_pStatic->SetText( m_strShowText.c_str() );
}

bool CWndEdit::CreateNoFont( int x, int y, int cx, int cy, CWndBase* pParent, int nID )
{
	return CWndEdit::Create(x,y,cx,cy,"",pParent,nID,m_pHgeFont,HGETEXT_LEFT);
}

float CWndEdit::GetStringWidth( const char* pChar, bool bMulti )
{
	return m_pStatic->GetStringWidth( pChar, bMulti );
}

void CWndEdit::SetNotifyParent( bool bNotify )
{
	m_bNotifyParent = bNotify;
}

const char* CWndEdit::GetText() const
{
	return m_strText.c_str();
}
void CWndEdit::SetMask( const char* pMask )
{
	m_strMask = pMask;
}
int CWndEdit::OnLButtonDown(const UINT &nFlags,const POINT &point)
{
	if(PtInWnd(point))
	{
		SetFocus(this);
		return WND_RESULT_YES;
	}
	return WND_RESULT_NO;
}