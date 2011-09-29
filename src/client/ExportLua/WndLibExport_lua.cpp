/*
** Lua binding: WndLibExport
** Generated automatically by tolua++-1.0.92 on 09/29/11 14:14:16.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_WndLibExport_open (lua_State* tolua_S);

#include "../wndlib/WndBase.h"
#include "../wndlib/wndpicture.h"
#include "../wndlib/wnddesktop.h"
#include "../wndlib/wndloadpicture.h"
#include "../wndlib/wndstatic.h"
#include "../wndlib/wndedit.h"
#include "../UI/UIExportWndBase.h"
#include "../ExportLua/UIGlobalExport.h"
#include "wtypes.h"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_CWndDesktop (lua_State* tolua_S)
{
 CWndDesktop* self = (CWndDesktop*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_CWndBase (lua_State* tolua_S)
{
 CWndBase* self = (CWndBase*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_CWndStatic (lua_State* tolua_S)
{
 CWndStatic* self = (CWndStatic*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_CWndLoadPicture (lua_State* tolua_S)
{
 CWndLoadPicture* self = (CWndLoadPicture*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_CWndPicture (lua_State* tolua_S)
{
 CWndPicture* self = (CWndPicture*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_CUIExportWndBase (lua_State* tolua_S)
{
 CUIExportWndBase* self = (CUIExportWndBase*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_POINT (lua_State* tolua_S)
{
 POINT* self = (POINT*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_CWndEdit (lua_State* tolua_S)
{
 CWndEdit* self = (CWndEdit*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_SIZE (lua_State* tolua_S)
{
 SIZE* self = (SIZE*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"CWndDesktop");
 tolua_usertype(tolua_S,"CWndBase");
 tolua_usertype(tolua_S,"CWndStatic");
 tolua_usertype(tolua_S,"CWndLoadPicture");
 tolua_usertype(tolua_S,"CWndPicture");
 tolua_usertype(tolua_S,"CUIExportWndBase");
 tolua_usertype(tolua_S,"POINT");
 tolua_usertype(tolua_S,"CWndEdit");
 tolua_usertype(tolua_S,"SIZE");
}

/* get function: cx of class  SIZE */
#ifndef TOLUA_DISABLE_tolua_get_SIZE_cx
static int tolua_get_SIZE_cx(lua_State* tolua_S)
{
  SIZE* self = (SIZE*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'cx'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->cx);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: cx of class  SIZE */
#ifndef TOLUA_DISABLE_tolua_set_SIZE_cx
static int tolua_set_SIZE_cx(lua_State* tolua_S)
{
  SIZE* self = (SIZE*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'cx'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->cx = ((  int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: cy of class  SIZE */
#ifndef TOLUA_DISABLE_tolua_get_SIZE_cy
static int tolua_get_SIZE_cy(lua_State* tolua_S)
{
  SIZE* self = (SIZE*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'cy'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->cy);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: cy of class  SIZE */
#ifndef TOLUA_DISABLE_tolua_set_SIZE_cy
static int tolua_set_SIZE_cy(lua_State* tolua_S)
{
  SIZE* self = (SIZE*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'cy'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->cy = ((  int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: x of class  POINT */
#ifndef TOLUA_DISABLE_tolua_get_POINT_x
static int tolua_get_POINT_x(lua_State* tolua_S)
{
  POINT* self = (POINT*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->x);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: x of class  POINT */
#ifndef TOLUA_DISABLE_tolua_set_POINT_x
static int tolua_set_POINT_x(lua_State* tolua_S)
{
  POINT* self = (POINT*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->x = ((  long)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: y of class  POINT */
#ifndef TOLUA_DISABLE_tolua_get_POINT_y
static int tolua_get_POINT_y(lua_State* tolua_S)
{
  POINT* self = (POINT*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->y);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: y of class  POINT */
#ifndef TOLUA_DISABLE_tolua_set_POINT_y
static int tolua_set_POINT_y(lua_State* tolua_S)
{
  POINT* self = (POINT*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->y = ((  long)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  CWndBase */
#ifndef TOLUA_DISABLE_tolua_WndLibExport_CWndBase_new00
static int tolua_WndLibExport_CWndBase_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CWndBase",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CWndBase* tolua_ret = (CWndBase*)  Mtolua_new((CWndBase)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CWndBase");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  CWndBase */
#ifndef TOLUA_DISABLE_tolua_WndLibExport_CWndBase_new00_local
static int tolua_WndLibExport_CWndBase_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CWndBase",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CWndBase* tolua_ret = (CWndBase*)  Mtolua_new((CWndBase)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CWndBase");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  CWndBase */
#ifndef TOLUA_DISABLE_tolua_WndLibExport_CWndBase_delete00
static int tolua_WndLibExport_CWndBase_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CWndBase",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CWndBase* self = (CWndBase*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Create of class  CWndBase */
#ifndef TOLUA_DISABLE_tolua_WndLibExport_CWndBase_Create00
static int tolua_WndLibExport_CWndBase_Create00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CWndBase",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,6,"CWndBase",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,7,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,8,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CWndBase* self = (CWndBase*)  tolua_tousertype(tolua_S,1,0);
  int x = ((int)  tolua_tonumber(tolua_S,2,0));
  int y = ((int)  tolua_tonumber(tolua_S,3,0));
  int cx = ((int)  tolua_tonumber(tolua_S,4,0));
  int cy = ((int)  tolua_tonumber(tolua_S,5,0));
  CWndBase* pParent = ((CWndBase*)  tolua_tousertype(tolua_S,6,0));
  int nID = ((int)  tolua_tonumber(tolua_S,7,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Create'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->Create(x,y,cx,cy,pParent,nID);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Create'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: MoveWindow of class  CWndBase */
#ifndef TOLUA_DISABLE_tolua_WndLibExport_CWndBase_MoveWindow00
static int tolua_WndLibExport_CWndBase_MoveWindow00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CWndBase",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CWndBase* self = (CWndBase*)  tolua_tousertype(tolua_S,1,0);
  int x = ((int)  tolua_tonumber(tolua_S,2,0));
  int y = ((int)  tolua_tonumber(tolua_S,3,0));
  bool bRedraw = ((bool)  tolua_toboolean(tolua_S,4,true));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'MoveWindow'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->MoveWindow(x,y,bRedraw);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'MoveWindow'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ResizeWindow of class  CWndBase */
#ifndef TOLUA_DISABLE_tolua_WndLibExport_CWndBase_ResizeWindow00
static int tolua_WndLibExport_CWndBase_ResizeWindow00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CWndBase",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CWndBase* self = (CWndBase*)  tolua_tousertype(tolua_S,1,0);
  int cx = ((int)  tolua_tonumber(tolua_S,2,0));
  int cy = ((int)  tolua_tonumber(tolua_S,3,0));
  bool bRedraw = ((bool)  tolua_toboolean(tolua_S,4,true));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ResizeWindow'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->ResizeWindow(cx,cy,bRedraw);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ResizeWindow'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetClientPos of class  CWndBase */
#ifndef TOLUA_DISABLE_tolua_WndLibExport_CWndBase_GetClientPos00
static int tolua_WndLibExport_CWndBase_GetClientPos00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const CWndBase",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const CWndBase* self = (const CWndBase*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetClientPos'", NULL);
#endif
  {
   POINT tolua_ret = (POINT)  self->GetClientPos();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((POINT)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"POINT");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(POINT));
     tolua_pushusertype(tolua_S,tolua_obj,"POINT");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetClientPos'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsWindowEnabled of class  CWndBase */
#ifndef TOLUA_DISABLE_tolua_WndLibExport_CWndBase_IsWindowEnabled00
static int tolua_WndLibExport_CWndBase_IsWindowEnabled00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CWndBase",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CWndBase* self = (CWndBase*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsWindowEnabled'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsWindowEnabled();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsWindowEnabled'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetClientSize of class  CWndBase */
#ifndef TOLUA_DISABLE_tolua_WndLibExport_CWndBase_GetClientSize00
static int tolua_WndLibExport_CWndBase_GetClientSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const CWndBase",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const CWndBase* self = (const CWndBase*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetClientSize'", NULL);
#endif
  {
   SIZE tolua_ret = (SIZE)  self->GetClientSize();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((SIZE)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"SIZE");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(SIZE));
     tolua_pushusertype(tolua_S,tolua_obj,"SIZE");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetClientSize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ShowWindow of class  CWndBase */
#ifndef TOLUA_DISABLE_tolua_WndLibExport_CWndBase_ShowWindow00
static int tolua_WndLibExport_CWndBase_ShowWindow00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CWndBase",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CWndBase* self = (CWndBase*)  tolua_tousertype(tolua_S,1,0);
  bool bVisible = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ShowWindow'", NULL);
#endif
  {
   self->ShowWindow(bVisible);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ShowWindow'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetFocus of class  CWndBase */
#ifndef TOLUA_DISABLE_tolua_WndLibExport_CWndBase_SetFocus00
static int tolua_WndLibExport_CWndBase_SetFocus00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CWndBase",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CWndBase",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CWndBase* self = (CWndBase*)  tolua_tousertype(tolua_S,1,0);
  CWndBase* pWndFocus = ((CWndBase*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetFocus'", NULL);
#endif
  {
   CWndBase* tolua_ret = (CWndBase*)  self->SetFocus(pWndFocus);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CWndBase");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetFocus'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  CWndPicture */
#ifndef TOLUA_DISABLE_tolua_WndLibExport_CWndPicture_new00
static int tolua_WndLibExport_CWndPicture_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CWndPicture",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CWndPicture* tolua_ret = (CWndPicture*)  Mtolua_new((CWndPicture)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CWndPicture");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  CWndPicture */
#ifndef TOLUA_DISABLE_tolua_WndLibExport_CWndPicture_new00_local
static int tolua_WndLibExport_CWndPicture_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CWndPicture",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CWndPicture* tolua_ret = (CWndPicture*)  Mtolua_new((CWndPicture)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CWndPicture");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  CWndPicture */
#ifndef TOLUA_DISABLE_tolua_WndLibExport_CWndPicture_delete00
static int tolua_WndLibExport_CWndPicture_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CWndPicture",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CWndPicture* self = (CWndPicture*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Create of class  CWndPicture */
#ifndef TOLUA_DISABLE_tolua_WndLibExport_CWndPicture_Create00
static int tolua_WndLibExport_CWndPicture_Create00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CWndPicture",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"CWndBase",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CWndPicture* self = (CWndPicture*)  tolua_tousertype(tolua_S,1,0);
  int x = ((int)  tolua_tonumber(tolua_S,2,0));
  int y = ((int)  tolua_tonumber(tolua_S,3,0));
  const char* pKey = ((const char*)  tolua_tostring(tolua_S,4,0));
  CWndBase* pParent = ((CWndBase*)  tolua_tousertype(tolua_S,5,0));
  int nID = ((int)  tolua_tonumber(tolua_S,6,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Create'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->Create(x,y,pKey,pParent,nID);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Create'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  CWndDesktop */
#ifndef TOLUA_DISABLE_tolua_WndLibExport_CWndDesktop_new00
static int tolua_WndLibExport_CWndDesktop_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CWndDesktop",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CWndDesktop* tolua_ret = (CWndDesktop*)  Mtolua_new((CWndDesktop)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CWndDesktop");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  CWndDesktop */
#ifndef TOLUA_DISABLE_tolua_WndLibExport_CWndDesktop_new00_local
static int tolua_WndLibExport_CWndDesktop_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CWndDesktop",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CWndDesktop* tolua_ret = (CWndDesktop*)  Mtolua_new((CWndDesktop)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CWndDesktop");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  CWndDesktop */
#ifndef TOLUA_DISABLE_tolua_WndLibExport_CWndDesktop_delete00
static int tolua_WndLibExport_CWndDesktop_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CWndDesktop",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CWndDesktop* self = (CWndDesktop*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  CUIExportWndBase */
#ifndef TOLUA_DISABLE_tolua_WndLibExport_CUIExportWndBase_new00
static int tolua_WndLibExport_CUIExportWndBase_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CUIExportWndBase",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CUIExportWndBase* tolua_ret = (CUIExportWndBase*)  Mtolua_new((CUIExportWndBase)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CUIExportWndBase");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  CUIExportWndBase */
#ifndef TOLUA_DISABLE_tolua_WndLibExport_CUIExportWndBase_new00_local
static int tolua_WndLibExport_CUIExportWndBase_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CUIExportWndBase",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CUIExportWndBase* tolua_ret = (CUIExportWndBase*)  Mtolua_new((CUIExportWndBase)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CUIExportWndBase");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  CUIExportWndBase */
#ifndef TOLUA_DISABLE_tolua_WndLibExport_CUIExportWndBase_delete00
static int tolua_WndLibExport_CUIExportWndBase_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CUIExportWndBase",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CUIExportWndBase* self = (CUIExportWndBase*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Create of class  CUIExportWndBase */
#ifndef TOLUA_DISABLE_tolua_WndLibExport_CUIExportWndBase_Create00
static int tolua_WndLibExport_CUIExportWndBase_Create00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CUIExportWndBase",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,6,"CWndBase",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,7,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,8,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CUIExportWndBase* self = (CUIExportWndBase*)  tolua_tousertype(tolua_S,1,0);
  int x = ((int)  tolua_tonumber(tolua_S,2,0));
  int y = ((int)  tolua_tonumber(tolua_S,3,0));
  int cx = ((int)  tolua_tonumber(tolua_S,4,0));
  int cy = ((int)  tolua_tonumber(tolua_S,5,0));
  CWndBase* pParent = ((CWndBase*)  tolua_tousertype(tolua_S,6,0));
  int nID = ((int)  tolua_tonumber(tolua_S,7,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Create'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->Create(x,y,cx,cy,pParent,nID);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Create'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetScript of class  CUIExportWndBase */
#ifndef TOLUA_DISABLE_tolua_WndLibExport_CUIExportWndBase_SetScript00
static int tolua_WndLibExport_CUIExportWndBase_SetScript00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CUIExportWndBase",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CUIExportWndBase* self = (CUIExportWndBase*)  tolua_tousertype(tolua_S,1,0);
  lua_State* pState =  tolua_S;
  const char* pUIEvent = ((const char*)  tolua_tostring(tolua_S,2,0));
  const char* pUITableName = ((const char*)  tolua_tostring(tolua_S,3,0));
  lua_Object nv = ((lua_Object)  tolua_tovalue(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetScript'", NULL);
#endif
  {
   self->SetScript(pState,pUIEvent,pUITableName,nv);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetScript'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  CWndLoadPicture */
#ifndef TOLUA_DISABLE_tolua_WndLibExport_CWndLoadPicture_new00
static int tolua_WndLibExport_CWndLoadPicture_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CWndLoadPicture",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CWndLoadPicture* tolua_ret = (CWndLoadPicture*)  Mtolua_new((CWndLoadPicture)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CWndLoadPicture");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  CWndLoadPicture */
#ifndef TOLUA_DISABLE_tolua_WndLibExport_CWndLoadPicture_new00_local
static int tolua_WndLibExport_CWndLoadPicture_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CWndLoadPicture",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CWndLoadPicture* tolua_ret = (CWndLoadPicture*)  Mtolua_new((CWndLoadPicture)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CWndLoadPicture");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  CWndLoadPicture */
#ifndef TOLUA_DISABLE_tolua_WndLibExport_CWndLoadPicture_delete00
static int tolua_WndLibExport_CWndLoadPicture_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CWndLoadPicture",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CWndLoadPicture* self = (CWndLoadPicture*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Create of class  CWndLoadPicture */
#ifndef TOLUA_DISABLE_tolua_WndLibExport_CWndLoadPicture_Create00
static int tolua_WndLibExport_CWndLoadPicture_Create00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CWndLoadPicture",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"CWndBase",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CWndLoadPicture* self = (CWndLoadPicture*)  tolua_tousertype(tolua_S,1,0);
  int x = ((int)  tolua_tonumber(tolua_S,2,0));
  int y = ((int)  tolua_tonumber(tolua_S,3,0));
  const char* pPath = ((const char*)  tolua_tostring(tolua_S,4,0));
  CWndBase* pParent = ((CWndBase*)  tolua_tousertype(tolua_S,5,0));
  int nID = ((int)  tolua_tonumber(tolua_S,6,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Create'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->Create(x,y,pPath,pParent,nID);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Create'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ResetRes of class  CWndLoadPicture */
#ifndef TOLUA_DISABLE_tolua_WndLibExport_CWndLoadPicture_ResetRes00
static int tolua_WndLibExport_CWndLoadPicture_ResetRes00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CWndLoadPicture",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CWndLoadPicture* self = (CWndLoadPicture*)  tolua_tousertype(tolua_S,1,0);
  const char* pPath = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ResetRes'", NULL);
#endif
  {
   self->ResetRes(pPath);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ResetRes'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  CWndStatic */
#ifndef TOLUA_DISABLE_tolua_WndLibExport_CWndStatic_new00
static int tolua_WndLibExport_CWndStatic_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CWndStatic",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CWndStatic* tolua_ret = (CWndStatic*)  Mtolua_new((CWndStatic)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CWndStatic");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  CWndStatic */
#ifndef TOLUA_DISABLE_tolua_WndLibExport_CWndStatic_new00_local
static int tolua_WndLibExport_CWndStatic_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CWndStatic",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CWndStatic* tolua_ret = (CWndStatic*)  Mtolua_new((CWndStatic)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CWndStatic");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  CWndStatic */
#ifndef TOLUA_DISABLE_tolua_WndLibExport_CWndStatic_delete00
static int tolua_WndLibExport_CWndStatic_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CWndStatic",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CWndStatic* self = (CWndStatic*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: CreateNoFont of class  CWndStatic */
#ifndef TOLUA_DISABLE_tolua_WndLibExport_CWndStatic_CreateNoFont00
static int tolua_WndLibExport_CWndStatic_CreateNoFont00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CWndStatic",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isstring(tolua_S,6,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,7,"CWndBase",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,8,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,9,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CWndStatic* self = (CWndStatic*)  tolua_tousertype(tolua_S,1,0);
  int x = ((int)  tolua_tonumber(tolua_S,2,0));
  int y = ((int)  tolua_tonumber(tolua_S,3,0));
  int cx = ((int)  tolua_tonumber(tolua_S,4,0));
  int cy = ((int)  tolua_tonumber(tolua_S,5,0));
  const char* pText = ((const char*)  tolua_tostring(tolua_S,6,0));
  CWndBase* pParent = ((CWndBase*)  tolua_tousertype(tolua_S,7,0));
  int nID = ((int)  tolua_tonumber(tolua_S,8,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CreateNoFont'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->CreateNoFont(x,y,cx,cy,pText,pParent,nID);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CreateNoFont'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetText of class  CWndStatic */
#ifndef TOLUA_DISABLE_tolua_WndLibExport_CWndStatic_SetText00
static int tolua_WndLibExport_CWndStatic_SetText00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CWndStatic",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CWndStatic* self = (CWndStatic*)  tolua_tousertype(tolua_S,1,0);
  const char* pText = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetText'", NULL);
#endif
  {
   self->SetText(pText);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetText'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetFontColor of class  CWndStatic */
#ifndef TOLUA_DISABLE_tolua_WndLibExport_CWndStatic_SetFontColor00
static int tolua_WndLibExport_CWndStatic_SetFontColor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CWndStatic",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CWndStatic* self = (CWndStatic*)  tolua_tousertype(tolua_S,1,0);
  unsigned long drColor = (( unsigned long)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetFontColor'", NULL);
#endif
  {
   self->SetFontColor(drColor);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetFontColor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetText of class  CWndStatic */
#ifndef TOLUA_DISABLE_tolua_WndLibExport_CWndStatic_GetText00
static int tolua_WndLibExport_CWndStatic_GetText00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const CWndStatic",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const CWndStatic* self = (const CWndStatic*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetText'", NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->GetText();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetText'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetStringWidth of class  CWndStatic */
#ifndef TOLUA_DISABLE_tolua_WndLibExport_CWndStatic_GetStringWidth00
static int tolua_WndLibExport_CWndStatic_GetStringWidth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CWndStatic",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CWndStatic* self = (CWndStatic*)  tolua_tousertype(tolua_S,1,0);
  const char* pChar = ((const char*)  tolua_tostring(tolua_S,2,0));
  bool bMulti = ((bool)  tolua_toboolean(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetStringWidth'", NULL);
#endif
  {
   float tolua_ret = (float)  self->GetStringWidth(pChar,bMulti);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetStringWidth'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  CWndEdit */
#ifndef TOLUA_DISABLE_tolua_WndLibExport_CWndEdit_new00
static int tolua_WndLibExport_CWndEdit_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CWndEdit",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CWndEdit* tolua_ret = (CWndEdit*)  Mtolua_new((CWndEdit)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CWndEdit");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  CWndEdit */
#ifndef TOLUA_DISABLE_tolua_WndLibExport_CWndEdit_new00_local
static int tolua_WndLibExport_CWndEdit_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CWndEdit",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CWndEdit* tolua_ret = (CWndEdit*)  Mtolua_new((CWndEdit)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CWndEdit");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  CWndEdit */
#ifndef TOLUA_DISABLE_tolua_WndLibExport_CWndEdit_delete00
static int tolua_WndLibExport_CWndEdit_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CWndEdit",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CWndEdit* self = (CWndEdit*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: CreateNoFont of class  CWndEdit */
#ifndef TOLUA_DISABLE_tolua_WndLibExport_CWndEdit_CreateNoFont00
static int tolua_WndLibExport_CWndEdit_CreateNoFont00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CWndEdit",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,6,"CWndBase",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,7,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,8,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CWndEdit* self = (CWndEdit*)  tolua_tousertype(tolua_S,1,0);
  int x = ((int)  tolua_tonumber(tolua_S,2,0));
  int y = ((int)  tolua_tonumber(tolua_S,3,0));
  int cx = ((int)  tolua_tonumber(tolua_S,4,0));
  int cy = ((int)  tolua_tonumber(tolua_S,5,0));
  CWndBase* pParent = ((CWndBase*)  tolua_tousertype(tolua_S,6,0));
  int nID = ((int)  tolua_tonumber(tolua_S,7,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CreateNoFont'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->CreateNoFont(x,y,cx,cy,pParent,nID);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CreateNoFont'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetStringWidth of class  CWndEdit */
#ifndef TOLUA_DISABLE_tolua_WndLibExport_CWndEdit_GetStringWidth00
static int tolua_WndLibExport_CWndEdit_GetStringWidth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CWndEdit",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CWndEdit* self = (CWndEdit*)  tolua_tousertype(tolua_S,1,0);
  const char* pChar = ((const char*)  tolua_tostring(tolua_S,2,0));
  bool bMulti = ((bool)  tolua_toboolean(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetStringWidth'", NULL);
#endif
  {
   float tolua_ret = (float)  self->GetStringWidth(pChar,bMulti);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetStringWidth'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetNotifyParent of class  CWndEdit */
#ifndef TOLUA_DISABLE_tolua_WndLibExport_CWndEdit_SetNotifyParent00
static int tolua_WndLibExport_CWndEdit_SetNotifyParent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CWndEdit",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CWndEdit* self = (CWndEdit*)  tolua_tousertype(tolua_S,1,0);
  bool bNotify = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetNotifyParent'", NULL);
#endif
  {
   self->SetNotifyParent(bNotify);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetNotifyParent'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetText of class  CWndEdit */
#ifndef TOLUA_DISABLE_tolua_WndLibExport_CWndEdit_GetText00
static int tolua_WndLibExport_CWndEdit_GetText00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const CWndEdit",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const CWndEdit* self = (const CWndEdit*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetText'", NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->GetText();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetText'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetText of class  CWndEdit */
#ifndef TOLUA_DISABLE_tolua_WndLibExport_CWndEdit_SetText00
static int tolua_WndLibExport_CWndEdit_SetText00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CWndEdit",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CWndEdit* self = (CWndEdit*)  tolua_tousertype(tolua_S,1,0);
  const char* pText = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetText'", NULL);
#endif
  {
   self->SetText(pText);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetText'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: CreateBaseExportWnd */
#ifndef TOLUA_DISABLE_tolua_WndLibExport_CreateBaseExportWnd00
static int tolua_WndLibExport_CreateBaseExportWnd00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  lua_State* pState =  tolua_S;
  {
   CUIExportWndBase* tolua_ret = (CUIExportWndBase*)  CreateBaseExportWnd(pState);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CUIExportWndBase");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CreateBaseExportWnd'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: CreateUI */
#ifndef TOLUA_DISABLE_tolua_WndLibExport_CreateUI00
static int tolua_WndLibExport_CreateUI00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  lua_State* pState =  tolua_S;
  const char* pFileName = ((const char*)  tolua_tostring(tolua_S,1,0));
  {
   bool tolua_ret = (bool)  CreateUI(pState,pFileName);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CreateUI'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_WndLibExport_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"SIZE","SIZE","",tolua_collect_SIZE);
  #else
  tolua_cclass(tolua_S,"SIZE","SIZE","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"SIZE");
   tolua_variable(tolua_S,"cx",tolua_get_SIZE_cx,tolua_set_SIZE_cx);
   tolua_variable(tolua_S,"cy",tolua_get_SIZE_cy,tolua_set_SIZE_cy);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"POINT","POINT","",tolua_collect_POINT);
  #else
  tolua_cclass(tolua_S,"POINT","POINT","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"POINT");
   tolua_variable(tolua_S,"x",tolua_get_POINT_x,tolua_set_POINT_x);
   tolua_variable(tolua_S,"y",tolua_get_POINT_y,tolua_set_POINT_y);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"CWndBase","CWndBase","",tolua_collect_CWndBase);
  #else
  tolua_cclass(tolua_S,"CWndBase","CWndBase","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"CWndBase");
   tolua_function(tolua_S,"new",tolua_WndLibExport_CWndBase_new00);
   tolua_function(tolua_S,"new_local",tolua_WndLibExport_CWndBase_new00_local);
   tolua_function(tolua_S,".call",tolua_WndLibExport_CWndBase_new00_local);
   tolua_function(tolua_S,"delete",tolua_WndLibExport_CWndBase_delete00);
   tolua_function(tolua_S,"Create",tolua_WndLibExport_CWndBase_Create00);
   tolua_function(tolua_S,"MoveWindow",tolua_WndLibExport_CWndBase_MoveWindow00);
   tolua_function(tolua_S,"ResizeWindow",tolua_WndLibExport_CWndBase_ResizeWindow00);
   tolua_function(tolua_S,"GetClientPos",tolua_WndLibExport_CWndBase_GetClientPos00);
   tolua_function(tolua_S,"IsWindowEnabled",tolua_WndLibExport_CWndBase_IsWindowEnabled00);
   tolua_function(tolua_S,"GetClientSize",tolua_WndLibExport_CWndBase_GetClientSize00);
   tolua_function(tolua_S,"ShowWindow",tolua_WndLibExport_CWndBase_ShowWindow00);
   tolua_function(tolua_S,"SetFocus",tolua_WndLibExport_CWndBase_SetFocus00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"CWndPicture","CWndPicture","",tolua_collect_CWndPicture);
  #else
  tolua_cclass(tolua_S,"CWndPicture","CWndPicture","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"CWndPicture");
   tolua_function(tolua_S,"new",tolua_WndLibExport_CWndPicture_new00);
   tolua_function(tolua_S,"new_local",tolua_WndLibExport_CWndPicture_new00_local);
   tolua_function(tolua_S,".call",tolua_WndLibExport_CWndPicture_new00_local);
   tolua_function(tolua_S,"delete",tolua_WndLibExport_CWndPicture_delete00);
   tolua_function(tolua_S,"Create",tolua_WndLibExport_CWndPicture_Create00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"CWndDesktop","CWndDesktop","",tolua_collect_CWndDesktop);
  #else
  tolua_cclass(tolua_S,"CWndDesktop","CWndDesktop","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"CWndDesktop");
   tolua_function(tolua_S,"new",tolua_WndLibExport_CWndDesktop_new00);
   tolua_function(tolua_S,"new_local",tolua_WndLibExport_CWndDesktop_new00_local);
   tolua_function(tolua_S,".call",tolua_WndLibExport_CWndDesktop_new00_local);
   tolua_function(tolua_S,"delete",tolua_WndLibExport_CWndDesktop_delete00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"CUIExportWndBase","CUIExportWndBase","",tolua_collect_CUIExportWndBase);
  #else
  tolua_cclass(tolua_S,"CUIExportWndBase","CUIExportWndBase","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"CUIExportWndBase");
   tolua_function(tolua_S,"new",tolua_WndLibExport_CUIExportWndBase_new00);
   tolua_function(tolua_S,"new_local",tolua_WndLibExport_CUIExportWndBase_new00_local);
   tolua_function(tolua_S,".call",tolua_WndLibExport_CUIExportWndBase_new00_local);
   tolua_function(tolua_S,"delete",tolua_WndLibExport_CUIExportWndBase_delete00);
   tolua_function(tolua_S,"Create",tolua_WndLibExport_CUIExportWndBase_Create00);
   tolua_function(tolua_S,"SetScript",tolua_WndLibExport_CUIExportWndBase_SetScript00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"CWndLoadPicture","CWndLoadPicture","",tolua_collect_CWndLoadPicture);
  #else
  tolua_cclass(tolua_S,"CWndLoadPicture","CWndLoadPicture","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"CWndLoadPicture");
   tolua_function(tolua_S,"new",tolua_WndLibExport_CWndLoadPicture_new00);
   tolua_function(tolua_S,"new_local",tolua_WndLibExport_CWndLoadPicture_new00_local);
   tolua_function(tolua_S,".call",tolua_WndLibExport_CWndLoadPicture_new00_local);
   tolua_function(tolua_S,"delete",tolua_WndLibExport_CWndLoadPicture_delete00);
   tolua_function(tolua_S,"Create",tolua_WndLibExport_CWndLoadPicture_Create00);
   tolua_function(tolua_S,"ResetRes",tolua_WndLibExport_CWndLoadPicture_ResetRes00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"CWndStatic","CWndStatic","",tolua_collect_CWndStatic);
  #else
  tolua_cclass(tolua_S,"CWndStatic","CWndStatic","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"CWndStatic");
   tolua_function(tolua_S,"new",tolua_WndLibExport_CWndStatic_new00);
   tolua_function(tolua_S,"new_local",tolua_WndLibExport_CWndStatic_new00_local);
   tolua_function(tolua_S,".call",tolua_WndLibExport_CWndStatic_new00_local);
   tolua_function(tolua_S,"delete",tolua_WndLibExport_CWndStatic_delete00);
   tolua_function(tolua_S,"CreateNoFont",tolua_WndLibExport_CWndStatic_CreateNoFont00);
   tolua_function(tolua_S,"SetText",tolua_WndLibExport_CWndStatic_SetText00);
   tolua_function(tolua_S,"SetFontColor",tolua_WndLibExport_CWndStatic_SetFontColor00);
   tolua_function(tolua_S,"GetText",tolua_WndLibExport_CWndStatic_GetText00);
   tolua_function(tolua_S,"GetStringWidth",tolua_WndLibExport_CWndStatic_GetStringWidth00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"CWndEdit","CWndEdit","",tolua_collect_CWndEdit);
  #else
  tolua_cclass(tolua_S,"CWndEdit","CWndEdit","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"CWndEdit");
   tolua_function(tolua_S,"new",tolua_WndLibExport_CWndEdit_new00);
   tolua_function(tolua_S,"new_local",tolua_WndLibExport_CWndEdit_new00_local);
   tolua_function(tolua_S,".call",tolua_WndLibExport_CWndEdit_new00_local);
   tolua_function(tolua_S,"delete",tolua_WndLibExport_CWndEdit_delete00);
   tolua_function(tolua_S,"CreateNoFont",tolua_WndLibExport_CWndEdit_CreateNoFont00);
   tolua_function(tolua_S,"GetStringWidth",tolua_WndLibExport_CWndEdit_GetStringWidth00);
   tolua_function(tolua_S,"SetNotifyParent",tolua_WndLibExport_CWndEdit_SetNotifyParent00);
   tolua_function(tolua_S,"GetText",tolua_WndLibExport_CWndEdit_GetText00);
   tolua_function(tolua_S,"SetText",tolua_WndLibExport_CWndEdit_SetText00);
  tolua_endmodule(tolua_S);
  tolua_function(tolua_S,"CreateBaseExportWnd",tolua_WndLibExport_CreateBaseExportWnd00);
  tolua_function(tolua_S,"CreateUI",tolua_WndLibExport_CreateUI00);

  { /* begin embedded lua code */
   int top = lua_gettop(tolua_S);
   static const unsigned char B[] = {
    10, 98, 97,115,101,119,110,100, 61,123,125, 10,102,117,110,
     99,116,105,111,110, 32, 98, 97,115,101,119,110,100, 46,116,
    111, 66, 97,115,101, 40,101, 41, 10,114,101,116,117,114,110,
     32,116,111,108,117, 97, 46, 99, 97,115,116, 40,101, 44, 32,
     34, 67, 87,110,100, 66, 97,115,101, 34, 41, 10,101,110,100,
     45, 45, 45, 45, 45,32
   };
   tolua_dobuffer(tolua_S,(char*)B,sizeof(B),"tolua: embedded Lua code 1");
   lua_settop(tolua_S, top);
  } /* end of embedded lua code */

 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_WndLibExport (lua_State* tolua_S) {
 return tolua_WndLibExport_open(tolua_S);
};
#endif

