#include "WndBase.h"
#include "../GraphicDefine/datadefine.h"
#include "../hgedevice/HGEDevice.h"
#include "wnddatadefine.h"
#include <hge.h>
#include <hgefont.h>
#include <string>
#include <algorithm>
HGEDevice* CWndBase::m_pDevice = NULL;

CWndBase* CWndBase::m_pWndCursorInLast = NULL;
CWndBase* CWndBase::m_pWndCursorIn = NULL;
CWndBase* CWndBase::s_pWndLBDown = NULL;
CWndBase* CWndBase::m_pWndFocus = NULL;
hgeFont* CWndBase::m_pHgeFont = NULL;

bool CWndBase::m_bLPressed = false;
bool CWndBase::m_bLReleased = false;
bool CWndBase::m_bRPressed = false;
bool CWndBase::m_bRReleased = false;

POINT CWndBase::m_ptMouse = {0,0};

CWndBase::CWndBase(void)
:m_nDlgID( 0 ),m_bDrawBox(true),m_bVisible(true),m_bDragWithParent(true),m_pParent(NULL)
,m_bNeedMouseInput(true)
{
	SetRectEmpty(&m_rcRect);
	SetRectEmpty(&m_rcClip);
	SetRectEmpty(&m_rcAbsWnd);	
	m_size.cx = 0;
	m_size.cy = 0;
	m_ptMouse.x = 0;
	m_ptMouse.y = 0;

	m_bLPressed		= false;
	m_bLReleased	= false;
	m_bRPressed		= false;
	m_bRReleased	= false;

	m_pTabWnd = NULL;

}

CWndBase::~CWndBase(void)
{
}

bool CWndBase::Create( int x, int y, int cx, int cy, CWndBase* pParent, int nID )
{

	SetRect(&m_rcRect,x,y,x+cx,y+cy);
	SetParent( pParent );
	SetDlgID( nID );

	return true;
}

CWndBase* CWndBase::SetParent( CWndBase* pParent )
{
	if( pParent == m_pParent )
		return m_pParent;

	CWndBase* pOld = m_pParent;
	if( pOld )
	{
		pOld->RemoveChildWnd( this );
	}
	m_pParent = pParent;

	if( m_pParent != NULL )
	{
		m_pParent->m_listChildren.push_back( this );
		SetWindowPos(NULL,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}
	return pOld;
}

void CWndBase::RemoveChildWnd( CWndBase* pChild )
{

}

int CWndBase::SetDlgID( int nId )
{
	int nOld = m_nDlgID;

	m_nDlgID = nId;
	return m_nDlgID;
}

void CWndBase::OnDraw()
{
	if( m_bVisible )
	{
		DrawBox();
		std::list < CWndBase* >::iterator itChild = m_listChildren.begin();
		std::list < CWndBase* >::iterator itEnd = m_listChildren.end();
		for(; itChild != itEnd; ++itChild )
		{
			CWndBase* pWnd = (*itChild);
			if( pWnd->IsVisible() )
			{
				pWnd->OnDraw();
			}
		}
		float MouseX,MouseY;
		m_pDevice->hge->Input_GetMousePos(&MouseX,&MouseY);

		bool b = false;
	}
}

bool CWndBase::IsVisible()
{
	return m_bVisible;
}

void CWndBase::DrawBox()
{
	if( m_bVisible && m_bDrawBox )
	{
		RECT rcWnd;
		GetWindowRect(&rcWnd);
		if(!IsRectEmpty(&m_rcClip))
		{
			IntersectRect(&rcWnd, &m_rcClip,&rcWnd);
		}
		if( IsRectEmpty(&rcWnd))
		{
			return;
		}
		SColor rc(255,255,255,0);
		if( m_pDevice )
		{
			m_pDevice->Draw2DRect(rc,rcWnd);
		}
		//Draw2drectangle()
	}
}

void CWndBase::SetDevice( HGEDevice* pDevice )
{
	m_pDevice = pDevice;
}

bool CWndBase::GetWindowRect( LPRECT lpRect ) const
{
	*lpRect = m_rcAbsWnd;
	return true;
}

void CWndBase::CheckAbsWnd()
{
	m_rcAbsWnd = m_rcRect;
	if( m_bDragWithParent )
	{
		if( m_pParent )
		{
			OffsetRect( &m_rcAbsWnd, m_pParent->m_rcAbsWnd.left, m_pParent->m_rcAbsWnd.top);
		}
		for( std::list < CWndBase* >::iterator it = m_listChildren.begin(); it != m_listChildren.end(); ++it )
		{
			CWndBase* pWnd = (*it);
			if( pWnd->IsDragWithParent() )
			{
				pWnd->CheckAbsWnd();
			}
		}
	}
}

void CWndBase::OnUpdate( float ft )
{
	while (m_listChildrenDestroy.size()) 
	{
		if(m_listChildrenDestroy.front()->HasWnd(m_pWndCursorInLast))
			m_pWndCursorInLast = NULL;
		if(m_listChildrenDestroy.front()->HasWnd(m_pWndCursorIn))
			m_pWndCursorIn = NULL;

		delete m_listChildrenDestroy.front();
		m_listChildrenDestroy.pop_front();
	}	
	std::list< CWndBase* >::iterator itChild;
	for(itChild = m_listChildren.begin(); itChild != m_listChildren.end(); itChild++)
	{
		if((*itChild)->IsWindowEnabled() && (*itChild)->IsVisible() )
		{
			(*itChild)->OnUpdate(ft);
		}
	}
}

CWndBase * CWndBase::GetChildWndPtIn( const POINT &pt )
{
	CWndBase *pWnd = NULL;

	std::list< CWndBase* >::reverse_iterator itChild;
	for( itChild = m_listChildren.rbegin(); itChild != m_listChildren.rend(); itChild++)
	{
		pWnd = (*itChild);
		if( pWnd->IsVisible() && pWnd->NeedMouseInput() && pWnd->PtInWnd(pt) )return pWnd;
	}

	return NULL;
}

bool CWndBase::PtInWnd( const POINT &pt ) const
{
	std::list< CWndBase* >::const_iterator itChild;
	for(itChild = m_listChildren.begin(); itChild != m_listChildren.end(); itChild++)
	{
		if((*itChild)->IsVisible() && (*itChild)->PtInWnd(pt))
			return true;
	}
	RECT rcWnd;
	GetWindowRect(&rcWnd);
	if ( PtInRect(&rcWnd, pt))
	{
		return true;
	}
	return false;
}

int CWndBase::OnMouseInput()
{
	if(!IsVisible())
		return WND_RESULT_NO;

	int lRes = WND_RESULT_NO;

	CWndBase *pWnd = GetChildWndPtIn(m_ptMouse);
	if(!pWnd) 
	{
		m_pWndCursorIn = this;
	}

	if(!IsWindowEnabled())
		return WND_RESULT_YES;

	if(pWnd)
	{
		lRes = pWnd->OnMouseInput();
	}

	if(lRes == WND_RESULT_NO)
	{
		lRes = ProcessMouseInput();
	}

	return lRes;
}

int CWndBase::ProcessMouseInput()
{
	int lRes = WND_RESULT_NO;
	static CWndBase* s_pWndLBDown = NULL;
	int nFlags = 0;
	if(m_bLPressed)
	{
		lRes = OnLButtonDown(nFlags,m_ptMouse);
		s_pWndLBDown = this;
	}
	if(m_bLReleased)
	{
		lRes = OnLButtonUp(nFlags,m_ptMouse);
		if(s_pWndLBDown == this)
			OnLButtonClick(nFlags, m_ptMouse);
	}
	if(m_bRPressed)
	{
		lRes = OnRButtonDown(nFlags, m_ptMouse);
	}
	if(m_bRReleased)
	{
		lRes = OnRButtonUp(nFlags,m_ptMouse);
	}
	if ( lRes == WND_RESULT_NO )
	{
		lRes = OnMouseMove(nFlags, m_ptMouse);
	}
	return lRes;
}

int CWndBase::OnRButtonDown( const UINT &nFlags,const POINT &point )
{
	return WND_RESULT_NO;
}

int CWndBase::OnRButtonUp( const UINT &nFlags,const POINT &point )
{
	return WND_RESULT_NO;
}

int CWndBase::OnLButtonDown( const UINT &nFlags,const POINT &point )
{
	return WND_RESULT_NO;
}

int CWndBase::OnLButtonUp( const UINT &nFlags,const POINT &point )
{
	return WND_RESULT_NO;
}

int CWndBase::OnLButtonClick( const UINT &nFlags, const POINT &point )
{
	return WND_RESULT_NO;
}

int CWndBase::OnMouseMove( const UINT &nFlags,const POINT &point )
{
	return WND_RESULT_NO;
}

int CWndBase::OnMouseMoveOut( const UINT &nFlags,const POINT &point )
{
	return WND_RESULT_NO;
}

bool CWndBase::IsWindowEnabled()
{
	return true;
}

int CWndBase::OnMouseMoveIn( const UINT &nFlags,const POINT &point )
{
	return WND_RESULT_NO;
}

bool CWndBase::ResizeWindow( int cx, int cy, bool bRedraw /*= true*/ )
{
	return SetWindowPos(NULL,0,0,cx,cy,SWP_NOMOVE);
}

bool CWndBase::MoveWindow( int x, int y, bool bRedraw /*= true*/ )
{
	return SetWindowPos(NULL,x,y,0,0,SWP_NOSIZE);
}

//不是move就是resize
bool CWndBase::SetWindowPos( CWndBase* pWndInsertAfter, int x, int y, int cx, int cy,UINT flag )
{
	if ((flag & SWP_NOMOVE) && (flag & SWP_NOSIZE))
	{

	}
	else if ( flag & SWP_NOMOVE ) 
	{
		m_rcRect.right = m_rcRect.left + cx;
		m_rcRect.bottom = m_rcRect.top + cy;
	}
	else if ( flag & SWP_NOSIZE ) 
	{
		m_rcRect.right += x - m_rcRect.left;
		m_rcRect.left = x;
		m_rcRect.bottom += y - m_rcRect.top;
		m_rcRect.top = y;
	}
	else
	{
		SetRect(&m_rcRect, x, y, x+cx, y+cy);
	}
	m_size.cx = m_rcRect.right - m_rcRect.left;
	m_size.cy = m_rcRect.bottom - m_rcRect.top;
	CheckAbsWnd();
	return true;
}

bool CWndBase::Destroy()
{	
	if(m_pParent != NULL)
	{
		m_pParent->m_listChildrenDestroy.push_back(this);

		std::list< CWndBase* >::iterator itChild = find(m_pParent->m_listChildren.begin(), m_pParent->m_listChildren.end(), this);
		if (itChild != m_pParent->m_listChildren.end()) 
		{
			m_pParent->m_listChildren.erase(itChild);
		}
	}
	return true;
}

bool CWndBase::HasWnd( CWndBase* pWnd )
{
	if(pWnd == NULL)
		return false;

	if(pWnd == this)
		return true;
	else
	{
		std::list<CWndBase*>::iterator itChild;
		for(itChild = m_listChildren.begin();itChild != m_listChildren.end(); ++itChild)
		{
			if((*itChild)->HasWnd(pWnd))
				return true;
		}
	}

	return false;
}

int CWndBase::OnKeyboardMessage( const hgeInputEvent& rEvent )
{
	int nRes = WND_RESULT_NO;
	if(!IsVisible() || !IsWindowEnabled())
		return nRes;


	std::list< CWndBase* >::reverse_iterator itChild = m_listChildren.rbegin();
	while( nRes  == WND_RESULT_NO && itChild != m_listChildren.rend())
	{
		nRes = (*itChild)->OnKeyboardMessage( rEvent );
		itChild ++;
	}

	if( nRes == WND_RESULT_NO )
	{
		nRes = ProcessKeyboardMessage( rEvent );
	}

	return nRes;
}

int CWndBase::ProcessKeyboardMessage( const hgeInputEvent& rEvent )
{
	int nRes = WND_RESULT_NO;
	if(  rEvent.type == INPUT_KEYDOWN || rEvent.type == INPUT_KEYUP )
	{
		if( rEvent.chr > 0 && rEvent.type == INPUT_KEYDOWN  )
		{
			nRes = OnChar( rEvent );
		}
		if( nRes == WND_RESULT_NO )
		{
			if( rEvent.type == INPUT_KEYDOWN  )
			{
				nRes = OnKeyDown( rEvent );
			}
			else if( rEvent.type == INPUT_KEYUP  )
			{
				nRes = OnKeyUp( rEvent );
			}
		}
	}

	return nRes;
}

int CWndBase::OnKeyDown( const hgeInputEvent& rEvent )
{
	int nRes = WND_RESULT_NO;
	return nRes;
}

int CWndBase::OnKeyUp( const hgeInputEvent& rEvent )
{
	int nRes = WND_RESULT_NO;
	if( m_pTabWnd )
	{
		if( rEvent.key == HGEK_TAB )
		{
			SetFocus(m_pTabWnd);
			return WND_RESULT_YES;
		}
	}
	return nRes;
}

int CWndBase::OnChar( const hgeInputEvent& rEvent )
{
	int nRes = WND_RESULT_NO;
	return nRes;
}

CWndBase* CWndBase::SetFocus( CWndBase* pWndFocus )
{
	if(m_pWndFocus == pWndFocus)
		return m_pWndFocus;

	CWndBase* pWndPrev = m_pWndFocus;
	m_pWndFocus = pWndFocus; 
	if(m_pWndFocus)
	{
		m_pWndFocus->OnSetFocus();
	}
	if(pWndPrev)	
	{
		pWndPrev->OnKillFocus();
	}
	return pWndPrev;
}

void CWndBase::ShowWindow( bool bVisible )
{
	m_bVisible = bVisible;
}

POINT CWndBase::GetClientPos() const
{
	POINT pt;
	pt.x = m_rcRect.left;
	pt.y = m_rcRect.top;
	return pt;
}

SIZE CWndBase::GetClientSize() const
{
	SIZE sizeReturnValue = {m_rcRect.right - m_rcRect.left,m_rcRect.bottom - m_rcRect.top};
	return sizeReturnValue;
}

void CWndBase::SetDragWithParent( bool bWith )
{
	m_bDragWithParent = bWith;
}

bool CWndBase::IsDragWithParent()
{
	return m_bDragWithParent;
}
bool CWndBase::NeedMouseInput()
{
	return m_bNeedMouseInput;
}
void CWndBase::SetTabWnd( CWndBase* pTabWnd )
{
	m_pTabWnd = pTabWnd;
}

int CWndBase::OnWndMessage( const int& nUIEvent, const int& nID )
{
	int nResult = WND_RESULT_NO;
	if ( m_pParent )
	{
		nResult = m_pParent->OnWndMessage( nUIEvent, nID );
	}
	else
	{

	}
	return nResult;
}

int CWndBase::SendMessage( const int& nUIEvent, const int& nID )
{
	return OnWndMessage( nUIEvent, nID );
}

int CWndBase::SendMessageToParent( const int& nUIEvent, const int& nID )
{
	int nRes = WND_RESULT_NO;
	if( m_pParent )
	{
		nRes = m_pParent->SendMessage( nUIEvent, nID );
	}
	return nRes;
}