#pragma once
#include <list>
#include <wtypes.h>
class HGEDevice;
class CWndBase
{
public:
	CWndBase(void);
	virtual ~CWndBase(void);
	virtual bool Create( int x, int y, int cx, int cy, CWndBase* pParent, int nID );
	CWndBase* SetParent( CWndBase* pParent );
	int SetDlgID( int nId );
	virtual void OnDraw();
	virtual bool IsVisible();
	virtual void DrawBox();
	static void SetDevice( HGEDevice* pDevice );
	bool GetWindowRect( LPRECT lpRect ) const;
protected:
	void RemoveChildWnd( CWndBase* pChild );
	void CheckAbsWnd();
protected:
	CWndBase* m_pParent;
	std::list < CWndBase* > m_listChildren;
	int m_nDlgID;
	RECT m_rcRect;
	RECT m_rcAbsWnd;
	bool m_bVisible;
	bool m_bDrawBox;
	RECT m_rcClip;

	static HGEDevice* m_pDevice;
};
