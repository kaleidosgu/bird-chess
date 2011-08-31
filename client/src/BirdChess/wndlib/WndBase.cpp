#include "WndBase.h"
#include "../GraphicDefine/datadefine.h"
#include "../hgedevice/HGEDevice.h"
HGEDevice* CWndBase::m_pDevice = NULL;
CWndBase::CWndBase(void)
:m_nDlgID( 0 ),m_bDrawBox(true),m_bVisible(true)
{
	SetRectEmpty(&m_rcRect);
	SetRectEmpty(&m_rcClip);
	SetRectEmpty(&m_rcAbsWnd);	
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
	}
	CheckAbsWnd();
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
		std::list < CWndBase* >::iterator itChild = m_listChildren.begin();
		std::list < CWndBase* >::iterator itEnd = m_listChildren.end();
		for(; itChild != itEnd; ++itChild )
		{
			CWndBase* pWnd = (*itChild);
			if( pWnd->IsVisible() )
			{
				pWnd->OnDraw();
				pWnd->DrawBox();
			}
		}
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
}

void CWndBase::OnUpdate()
{
	
}