#ifndef _WndButton_H_
#define _WndButton_H_
#include <vector>
#include "WndBase.h"
#include "wnddatadefine.h"

class CWndLoadPicture;
class CWndButton :
	public CWndBase
{
public:
	CWndButton(void);
	virtual ~CWndButton(void);
	virtual bool Create( int x, int y, int cx, int cy,const char* pPath, CWndBase* pParent, int nID );
	virtual int OnMouseMoveIn(const UINT &nFlags,const POINT &point);
	virtual int OnMouseMoveOut(const UINT &nFlags,const POINT &point);
	virtual int OnMouseMove(const UINT &nFlags,const POINT &point);
	virtual int OnLButtonDown(const UINT &nFlags,const POINT &point);
	virtual int OnLButtonUp(const UINT &nFlags,const POINT &point);
	virtual int OnLButtonClick(const UINT &nFlags, const POINT &point);

	virtual void SetBtnState( int nBtnState );
	virtual void OnDraw();

	void AddRes(int x, int y, int cx, int cy );
	void OnClick();
private:
	CWndLoadPicture* m_pPicture;
	std::vector < ResStruct > m_vecRes;
	int m_nBtnState;

	int m_nDrawX;
	int m_nDrawY;
	int m_nDrawCX;
	int m_nDrawCY;
	std::string m_strPath;
};

#endif

