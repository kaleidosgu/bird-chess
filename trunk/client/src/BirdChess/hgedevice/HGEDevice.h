#pragma once
#include "../GraphicDefine/datadefine.h"
#include <WTypes.h>
class HGE;
class hgeSprite;
class HGEDevice
{
public:
	HGEDevice(void);
	virtual ~HGEDevice(void);
	virtual void RenderLine( int x1, int y1, int x2, int y2,unsigned int cr );
	virtual void Draw2DRect( SColor color, RECT rcToDraw );

	HGE* hge;
private:
	hgeSprite* m_pSprite;
};
