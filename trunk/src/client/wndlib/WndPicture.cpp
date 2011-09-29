#include "WndPicture.h"
#include <hge.h>
#include <hgesprite.h>
#include "../hgedevice/HGEDevice.h"
#include "../EntityFactory.h"
#include "../lua/CLuaParse.h"
CWndPicture::CWndPicture(void):m_pSprite(NULL),m_nCurIndex(0),m_strRes("")
{
}

CWndPicture::~CWndPicture(void)
{
	delete m_pSprite;
	m_pSprite = NULL;
}

bool CWndPicture::Create( int x, int y, const char* pKey, CWndBase* pParent, int nID )
{
	int cx = 0;
	int cy = 0;
	bool bRes = false;
	if( m_pDevice ) 
	{
		m_strRes = pKey;
		CWndBase::Create(x,y,cx,cy,pParent,nID);
		SetTextureInfo(0);
	}
	else
	{
		//assert(false);
	}
	return bRes;
}
void CWndPicture::OnDraw()
{
	DrawBox();
	if( m_pSprite )
	{
		m_pSprite->Render(m_rcAbsWnd.left,m_rcAbsWnd.top);
	}
	CWndBase::OnDraw();
}

void CWndPicture::SetTextureInfo( int nIdx )
{
	m_nCurIndex = nIdx;
	int cx = 0;
	int cy = 0;
	int gx = 0;
	int gy = 0;
	bool bResEntity = EntityFactory::Instance().GetEntityList( m_lstSize, m_strRes.c_str() );
	if ( bResEntity )
	{
		m_Texture = EntityFactory::Instance().GetEntityTexture();
		EntitySize* pSize = NULL;
		int nSzSize = 0;
		for( std::list < EntitySize* >::iterator it = m_lstSize.begin(); it != m_lstSize.end(); ++it )
		{
			if( nSzSize == m_nCurIndex )
			{
				pSize = (*it);
				break;
			}
			nSzSize++;
		}
		if( pSize )
		{
			gx = pSize->x;
			gy = pSize->y;

			cx = pSize->w;
			cy = pSize->h;
		}
	}
	SetWindowPos(NULL,m_rcRect.left,m_rcRect.top,cx,cy,0);

	if( !m_pSprite )
	{
		m_pSprite = new hgeSprite(m_Texture,gx,gy,m_size.cx,m_size.cy);
	}
	else
	{
		m_pSprite->SetTextureRect(gx,gy,m_size.cx,m_size.cy);
	}
}
