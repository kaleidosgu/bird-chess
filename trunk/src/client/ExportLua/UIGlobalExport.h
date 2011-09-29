#ifndef _UIGLOBALEXPORT_H_
#define _UIGLOBALEXPORT_H_
struct lua_State;
class CUIExportWndBase;
CUIExportWndBase* CreateBaseExportWnd( lua_State* pState );
bool CreateUI( lua_State* pState, const char* pFileName );
#endif


