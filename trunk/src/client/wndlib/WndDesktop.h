#pragma once
#include "wndbase.h"
#include <hge.h>
class CWndDesktop :
	public CWndBase
{
public:
	CWndDesktop(void);
	virtual ~CWndDesktop(void);
	virtual void OnUpdate( float ft );
	virtual int OnMouseInput();
	virtual int OnKeyboardMessage( const hgeInputEvent& rEvent );
private:
	hgeInputEvent m_event;
	int m_nLastKey;
	bool bKeyProcess;
};
