#ifndef _WNDPICTURE_H__
#define _WNDPICTURE_H__
#include "wndbase.h"
#include "hge.h"
class hgeSprite;
struct EntitySize;
class CWndPicture :
	public CWndBase
{
public:
	CWndPicture(void);
	virtual ~CWndPicture(void);
	//x,y为坐标.cx,cy为尺寸.gx,gy是图素相对源文件的坐标.
	virtual bool Create( int x, int y, const char* pKey, CWndBase* pParent, int nID );
	virtual void OnDraw();
	void SetTextureInfo( int nIdx );
private:
	HTEXTURE m_Texture;
	hgeSprite* m_pSprite;
	std::list < EntitySize* > m_lstSize;
	int m_nCurIndex;
	std::string m_strRes;
};


#endif
