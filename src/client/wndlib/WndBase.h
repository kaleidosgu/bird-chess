#pragma once
#include <list>
#include <wtypes.h>
#include <hge.h>
class HGEDevice;
class hgeFont;
class CWndBase
{
public:
	CWndBase(void);
	virtual ~CWndBase(void);
	virtual bool Create( int x, int y, int cx, int cy, CWndBase* pParent, int nID );

	virtual void OnDraw();
	virtual bool IsVisible();
	virtual void DrawBox();
	virtual bool Destroy();

	virtual void OnUpdate( float ft );



	CWndBase* SetParent( CWndBase* pParent );
	int SetDlgID( int nId );
	static void SetDevice( HGEDevice* pDevice );
	bool GetWindowRect( LPRECT lpRect ) const;
	virtual int OnMouseInput();
	virtual bool PtInWnd(const POINT &pt) const;
	virtual bool IsWindowEnabled();

	virtual int OnRButtonDown(const UINT &nFlags,const POINT &point);
	virtual int OnRButtonUp(const UINT &nFlags,const POINT &point);
	virtual int OnLButtonDown(const UINT &nFlags,const POINT &point);
	virtual int OnLButtonUp(const UINT &nFlags,const POINT &point);
	virtual int OnLButtonClick(const UINT &nFlags, const POINT &point);
	virtual int OnMouseMove(const UINT &nFlags,const POINT &point);
	virtual int OnMouseMoveIn(const UINT &nFlags,const POINT &point);
	virtual int OnMouseMoveOut(const UINT &nFlags,const POINT &point);

	virtual int OnKeyboardMessage( const hgeInputEvent& rEvent );
	virtual int ProcessKeyboardMessage( const hgeInputEvent& rEvent );


	virtual int OnKeyDown( const hgeInputEvent& rEvent );
	virtual int OnKeyUp( const hgeInputEvent& rEvent );
	virtual int OnChar( const hgeInputEvent& rEvent );

	virtual bool ResizeWindow(int cx, int cy, bool bRedraw = true);
	virtual bool MoveWindow(int x, int y, bool bRedraw = true);

	virtual bool SetWindowPos(CWndBase* pWndInsertAfter, int x, int y, int cx, int cy, UINT flag );
	bool HasWnd(CWndBase* pWnd);

	static CWndBase* SetFocus(CWndBase* pWndFocus);

	virtual void ShowWindow( bool bVisible );
	POINT GetClientPos() const;
	SIZE GetClientSize() const;
	void SetDragWithParent( bool bWith );
	bool IsDragWithParent( );

	void SetTabWnd( CWndBase* pTabWnd );

	virtual int OnWndMessage( const int& nUIEvent, const int& nID );
	int SendMessage( const int& nUIEvent, const int& nID );
	int SendMessageToParent( const int& nUIEvent, const int& nID );

protected:
	virtual CWndBase * GetChildWndPtIn(const POINT &pt);
	void RemoveChildWnd( CWndBase* pChild );
	void CheckAbsWnd();
	virtual int ProcessMouseInput();
	virtual void OnSetFocus(){};
	virtual void OnKillFocus(){};
	virtual bool NeedMouseInput();
protected:
	CWndBase* m_pParent;
	std::list < CWndBase* > m_listChildren;
	int m_nDlgID;
	RECT m_rcRect;
	RECT m_rcAbsWnd;
	bool m_bVisible;
	RECT m_rcClip;

	SIZE m_size;

	static HGEDevice* m_pDevice;

	static bool m_bLPressed;
	static bool m_bLReleased; 
	static bool m_bRPressed;
	static bool m_bRReleased; 

	std::list < CWndBase* > m_listChildrenDestroy;

	bool	m_bDragWithParent;
	bool	m_bNeedMouseInput;

public:

	static CWndBase* m_pWndCursorIn;
	static CWndBase* m_pWndCursorInLast;
	static CWndBase* s_pWndLBDown;
	static CWndBase* m_pWndFocus;
	static POINT m_ptMouse;
	static hgeFont*		m_pHgeFont;
	bool	m_bDrawBox;

private:
	CWndBase*		m_pTabWnd;
};
