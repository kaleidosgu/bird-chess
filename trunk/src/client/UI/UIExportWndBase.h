#ifndef _UIExportWndBase_H_
#define _UIExportWndBase_H_
#include "../wndlib/WndBase.h"
#include <tolua++.h>
#include <map>
#include <string>
struct hgeInputEvent;
struct lua_State;
class CUIExportWndBase :
	public CWndBase
{
public:
	CUIExportWndBase(void);
	virtual ~CUIExportWndBase(void);
	virtual bool Create( int x, int y, int cx, int cy, CWndBase* pParent, int nID );
	virtual void OnUpdate( float ft );
	void SetScript( lua_State* pState, const char* pUIEvent, lua_Object nv);
	virtual int OnKeyDown( const hgeInputEvent& rEvent );
	virtual bool Destroy();
	int m_nCurTime;
private:
	bool _IsUIEventExist( int& nCallIndex, const char* pChar );
private:
	lua_State* m_pState;
	std::map < std::string, int > m_mapUIEventCall;
};

#endif

