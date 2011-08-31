#include "HGEDevice.h"
#include "hge.h"
HGEDevice::HGEDevice(void)
:hge(0),m_pSprite(0)
{
	hge = hgeCreate(HGE_VERSION);
}

HGEDevice::~HGEDevice(void)
{
}

void HGEDevice::RenderLine( int x1, int y1, int x2, int y2,unsigned int cr )
{
	float fx1 = (float)x1;
	float fx2 = (float)x2;
	float fy1 = (float)y1;
	float fy2 = (float)y2;
	hge->Gfx_RenderLine(fx1,fy1,fx2,fy2,cr);
}

void HGEDevice::Draw2DRect( SColor color, RECT rcToDraw )
{
	float x1,y1,x2,y2;
	x1 = (float)rcToDraw.left;
	y1 = (float)rcToDraw.top;
	x2 = (float)rcToDraw.right;
	y2 = (float)rcToDraw.bottom;

	DWORD dwColor = color.color;

	hge->Gfx_RenderLine(x1,y1,x2,y1,dwColor);
	hge->Gfx_RenderLine(x2,y1,x2,y2,dwColor);
	hge->Gfx_RenderLine(x2,y2,x1,y2,dwColor);
	hge->Gfx_RenderLine(x1,y2,x1,y1,dwColor);
}