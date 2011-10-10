#include "WndLoadPicture.h"
#include <hge.h>
#include <hgesprite.h>
#include "../hgedevice/HGEDevice.h"
CWndLoadPicture::CWndLoadPicture(void):m_pSprite(NULL)
{
	m_bNeedMouseInput = false;
}

CWndLoadPicture::~CWndLoadPicture(void)
{
	delete m_pSprite;
	m_pSprite = NULL;
}

bool CWndLoadPicture::Create( int x, int y, const char* pPath, CWndBase* pParent, int nID, bool bDrawSprite, int ptDrawX, int ptDrawY, int ptDrawW, int ptDrawH )
{
	bool bRes = false;
	if( m_pDevice ) 
	{
		ResetRes(pPath,bDrawSprite,ptDrawX,ptDrawY,ptDrawW,ptDrawH);
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

void CWndLoadPicture::ResetRes( const char* pPath, bool bDrawSprite, int ptDrawX, int ptDrawY, int ptDrawW, int ptDrawH )
{
	int xStart = 0;
	int yStart = 0;
	int cx = 0;
	int cy = 0;
	if( m_pDevice && m_pDevice->hge )
	{
		m_Texture = m_pDevice->hge->Texture_Load( pPath );

		if( bDrawSprite )
		{
			cx		= ptDrawW;
			cy		= ptDrawH;
			xStart	= ptDrawX;
			yStart	= ptDrawY;
		}
		else
		{
			cx = m_pDevice->hge->Texture_GetWidth( m_Texture );
			cy = m_pDevice->hge->Texture_GetHeight( m_Texture );
		}

		SetWindowPos(NULL,0,0,cx,cy,0);

		if( !m_pSprite )
		{
			m_pSprite = new hgeSprite( m_Texture, xStart, yStart, cx, cy );
		}
		else
		{
			m_pSprite->SetTextureRect( xStart, yStart, cx, cy );
		}
	}
}
void CWndLoadPicture::SetFlip(bool bX,bool bY,bool bHotSpot )
{
	if( m_pSprite )
	{
		m_pSprite->SetFlip( bX, bY, bHotSpot );
	}
}