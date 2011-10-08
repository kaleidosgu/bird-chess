#include "Wndbutton.h"

CWndButton::CWndButton(void)
{
}
CWndButton::~CWndButton(void)
{
}
bool CWndButton::Create( int x, int y, int cx, int cy, CWndBase* pParent, int nID )
{
	return CWndBase::Create(x,y,cx,cy,pParent,nID);
}