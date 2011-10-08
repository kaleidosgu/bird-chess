#ifndef _WndButton_H_
#define _WndButton_H_
#include "WndBase.h"
class CWndButton :
	public CWndBase
{
public:
	CWndButton(void);
	virtual ~CWndButton(void);
	virtual bool Create( int x, int y, int cx, int cy, CWndBase* pParent, int nID );
};

#endif

