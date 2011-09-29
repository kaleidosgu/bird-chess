#include "WndLoadPicture.h"
#include <hge.h>
#include <hgesprite.h>
#include "../hgedevice/HGEDevice.h"
CWndLoadPicture::CWndLoadPicture(void):m_pSprite(NULL)
{
}

CWndLoadPicture::~CWndLoadPicture(void)
{
	delete m_pSprite;
	m_pSprite = NULL;
}

bool CWndLoadPicture::Create( int x, int y, const char* pPath, CWndBase* pParent, int nID )
{
	bool bRes = false;
	if( m_pDevice ) 
	{
		ResetRes(pPath);
		CWndBase::Create( x,y,m_size.cx,m_size.cy,pParent,nID);
	}
	else
	{
		//assert(false);
	}
	return bRes;
}
void CWndLoadPicture::OnDraw()
{
	DrawBox();
	if( m_pSprite )
	{
		m_pSprite->Render(m_rcAbsWnd.left,m_rcAbsWnd.top);
	}
	CWndBase::OnDraw();
}

void CWndLoadPicture::ResetRes( const char* pPath )
{
	int cx = 0;
	int cy = 0;
	if( m_pDevice && m_pDevice->hge )
	{
		m_Texture = m_pDevice->hge->Texture_Load( pPath );

		cx = m_pDevice->hge->Texture_GetWidth( m_Texture );
		cy = m_pDevice->hge->Texture_GetHeight( m_Texture );

		SetWindowPos(NULL,0,0,cx,cy,0);

		if( !m_pSprite )
		{
			m_pSprite = new hgeSprite( m_Texture, 0, 0, cx, cy );
		}
		else
		{
			m_pSprite->SetTextureRect( 0, 0, cx, cy );
		}
	}
}