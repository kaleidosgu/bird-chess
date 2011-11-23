#ifndef _UIGLOBALEXPORT_H_
#define _UIGLOBALEXPORT_H_
struct lua_State;
class CUIExportWndBase;
CUIExportWndBase* CreateBaseExportWnd( lua_State* pState );
bool CreateUI( lua_State* pState, const char* pFileName );
void Login2Server( const char* pName, const char* pPassword);
unsigned long GetARGB( int a, int r, int g, int b );
void SendChatMsg( lua_State* pState, const char* pChatContent );
#endif


