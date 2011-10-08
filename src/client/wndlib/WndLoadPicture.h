#ifndef _WNDLOADPICTURE_H__
#define _WNDLOADPICTURE_H__
#include "wndbase.h"
#include "hge.h"
class hgeSprite;
class CWndLoadPicture :
	public CWndBase
{
public:
	CWndLoadPicture(void);
	virtual ~CWndLoadPicture(void);
	virtual bool Create( int x, int y, const char* pPath, CWndBase* pParent, int nID, bool bDrawSprite, int ptDrawX, int ptDrawY, int ptDrawW, int ptDrawH );
	virtual void OnDraw();
	void ResetRes( const char* pPath, bool bDrawSprite, int ptDrawX, int ptDrawY, int ptDrawW, int ptDrawH );
	void SetFlip(bool bX,bool bY,bool bHotSpot = false );
private:
	HTEXTURE m_Texture;
	hgeSprite* m_pSprite;
};


#endif
