//////////////////////////////////////////////////////////////////////////
//
// �༭�������
//
// NONO�޸���΢���ƽ��汾
//
//
//////////////////////////////////////////////////////////////////////////
#ifndef GDIEDIT_H
#define GDIEDIT_H

#include "GfxFont.h"


class GfxEdit
{
public:

	///
	/// nEditWidth		�༭�����
	///	nFontColor		������ɫ
	/// lpFontName		��������
	///	nFontSize		�����С
	/// bBold			�Ƿ����
	/// bItalic			�Ƿ�б��
	/// bAntialias		�Ƿ�ƽ��
	///
	GfxEdit(UINT nEditWidth, DWORD nFontColor, CHAR* lpFontName = "����", UINT nFontSize = 12, BOOL bBold = FALSE, BOOL bItalic = FALSE, BOOL bAntialias = TRUE);
	~GfxEdit(void);

	typedef enum tagDeletePos
	{
		DP_LEFT = 0,
		DP_RIGHT
	}TDP;

public:
	void			SetFocus(); // ���ñ༭�򽹵㣬ֻ��ӵ�б༭�򽹵������Ӧ����
	void			KillFocus(); // ����༭�򽹵㣬ʧȥ�༭�򽹵�󽫲�����Ӧ����

	void			ClearCookie(); // ����༭������

	void			InsertCookie(const char* lpCookie); // �����ַ������༭��ǰ��괦
	const char*		GetCookie(); // ��ȡ�༭��ǰ����

	void			InsertCookieW(const wchar_t* lpCookie); // �����ַ������༭��ǰ��괦(���ַ�ģʽ)
	const wchar_t*	GetCookieW(); // ��ȡ�༭��ǰ����(���ַ�ģʽ)

	void			Render(float x, float y); //


typedef void(*pGfxEditBackFun)(GfxEdit*);
	void			SetCallback(pGfxEditBackFun pFunc);

private:
	void			OnChar( UINT nChar, UINT nRepCnt, UINT nFlags );
	void			InsertChar(wchar_t aChar);
	void			DeleteChar(TDP aPos);

private:
	pGfxEditBackFun pBackFunc;
	static const int COOKIE_SIZE = 1024;
	wchar_t			m_sCookie[COOKIE_SIZE];

	FLOAT			m_nEditW;
	FLOAT			m_nEditH;

	UINT			m_nCharPos;		// �ַ�λ��
    
	HGE*			m_pHGE;			// HGEָ��
	hgeSprite*		m_pSprite;		// ��꾫��
	GfxFont*		m_pFont;		// ���徫��

	static LRESULT CALLBACK GfxEditWndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
};
#endif//GDIEDIT_H