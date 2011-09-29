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
	virtual bool Create( int x, int y, const char* pPath, CWndBase* pParent, int nID );
	virtual void OnDraw();
	void ResetRes( const char* pPath );
private:
	HTEXTURE m_Texture;
	hgeSprite* m_pSprite;
};


#endif
