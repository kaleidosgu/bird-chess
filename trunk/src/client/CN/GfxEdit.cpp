#include "GfxEdit.h"
#include <atlbase.h>

WNDPROC		g_lpLastHgeWndProc = NULL;
GfxEdit*	g_lpFocusEditPtr = NULL;

LRESULT	CALLBACK GfxEdit::GfxEditWndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	if (WM_KEYDOWN == nMsg || WM_KEYUP == nMsg)
	{
		MSG aMsg;
		memset(&aMsg,0,sizeof(MSG));

		aMsg.hwnd	= hWnd;
		aMsg.message= nMsg;
		aMsg.wParam	= wParam;
		aMsg.lParam	= lParam;
		aMsg.time	= GetTickCount();

		TranslateMessage(&aMsg);

		if (g_lpFocusEditPtr && WM_KEYDOWN == nMsg)
		{
			switch(wParam)
			{
			case VK_DELETE: //key delete
			case VK_LEFT: //key left
			case VK_RIGHT: //key right
			case VK_HOME: //key home
			case VK_END: //key end
				g_lpFocusEditPtr->OnChar(static_cast<UINT>(wParam), LOWORD(lParam), HIWORD(lParam));
				break;
			}
		}
	}
	else if (WM_CHAR == nMsg || WM_IME_CHAR == nMsg)
	{
		if (g_lpFocusEditPtr)
		{
			g_lpFocusEditPtr->OnChar(static_cast<UINT>(wParam), LOWORD(lParam), HIWORD(lParam));
			return TRUE;
		}
	}
	return g_lpLastHgeWndProc?g_lpLastHgeWndProc(hWnd,nMsg,wParam,lParam):TRUE;
}

GfxEdit::GfxEdit(UINT nEditWidth, DWORD nFontColor, CHAR* lpFontName, UINT nFontSize, BOOL bBold, BOOL bItalic, BOOL bAntialias)
{
	m_pHGE = hgeCreate(HGE_VERSION);
	if (!m_pHGE) throw NULL;

	HWND hWnd = m_pHGE->System_GetState(HGE_HWND);
	if (!hWnd) throw NULL;

	g_lpLastHgeWndProc = (WNDPROC)::GetWindowLong(hWnd,GWL_WNDPROC);
	if(g_lpLastHgeWndProc != (WNDPROC)::SetWindowLong(hWnd,GWL_WNDPROC,(LONG)GfxEditWndProc)) throw NULL;

	nEditWidth = (nEditWidth == 0 ? 1 : nEditWidth);
	nFontSize = (nFontSize == 0 ? 12 : nFontSize);

	m_nEditW = static_cast<FLOAT>(nEditWidth);
	m_nEditH = static_cast<FLOAT>(nFontSize);

	m_pSprite = new hgeSprite(NULL,0,0,1,m_nEditH);
	m_pSprite->SetColor(0xFFFF00FF);

	m_pFont = new GfxFont(lpFontName,nFontSize,bBold,bItalic,bAntialias);
	if (!m_pFont) throw NULL;
	m_pFont->SetColor(nFontColor);

	m_nCharPos = 0;

	ClearCookie();
}

GfxEdit::~GfxEdit(void)
{
	if(m_pHGE) m_pHGE->Release();
}

void GfxEdit::SetFocus()
{
	g_lpFocusEditPtr = this;
}

void GfxEdit::KillFocus()
{
	g_lpFocusEditPtr = NULL;
}

void GfxEdit::ClearCookie()
{
	memset(m_sCookie,0,sizeof(m_sCookie));
	m_nCharPos = 0;
}

void GfxEdit::InsertCookie(const char* lpCookie)
{
	InsertCookieW(CA2W(lpCookie));
}

const char* GfxEdit::GetCookie()
{
	static char sCookieA[COOKIE_SIZE];
	strcpy(sCookieA,CW2A(m_sCookie));
	return sCookieA;
}

void GfxEdit::InsertCookieW(const wchar_t* lpCookie)
{
	if (lpCookie)
	{
		while(*lpCookie)
		{
			InsertChar(*lpCookie++);
		}
	}
}

const wchar_t* GfxEdit::GetCookieW()
{
	return m_sCookie;
}

void GfxEdit::Render(float x, float y)
{
	float box_x = x - 1;
	float box_y = y - 1;
	float box_w = m_nEditW + 2;
	float box_h = m_nEditH + 2;

	// Íâ¿ò
	m_pHGE->Gfx_RenderLine(box_x, box_y, box_x + box_w, box_y);
	m_pHGE->Gfx_RenderLine(box_x, box_y, box_x, box_y + box_h);
	m_pHGE->Gfx_RenderLine(box_x + box_w, box_y + box_h, box_x + box_w, box_y);
	m_pHGE->Gfx_RenderLine(box_x + box_w, box_y + box_h, box_x, box_y + box_h);

	float text_x = x;
	float text_y = y;
	float char_x = x;
	float char_y = y;

	if (m_nCharPos > 0)
	{
		wchar_t sTmp[COOKIE_SIZE];
		memset(sTmp,0,sizeof(sTmp));
		wcsncpy(sTmp,m_sCookie,m_nCharPos);
		SIZE szTmp = m_pFont->GetTextSize(sTmp);
		float tmp_w = static_cast<float>(szTmp.cx);

		if (tmp_w > m_nEditW)
		{
			text_x -= (tmp_w - m_nEditW);
		}

		char_x = text_x + tmp_w;
	}

	int clip_x = static_cast<int>(box_x);
	int clip_y = static_cast<int>(box_y);
	int clip_w = static_cast<int>(box_w);
	int clip_h = static_cast<int>(box_h);

	m_pHGE->Gfx_SetClipping(clip_x, clip_y, clip_w, clip_h);
	m_pFont->Render(text_x,text_y,0,m_sCookie);
	m_pSprite->Render(char_x,char_y);
	m_pHGE->Gfx_SetClipping();
}

void GfxEdit::OnChar( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	if (VK_RETURN == nChar) // key enter
	{
		pBackFunc(this);
	}
	else if (VK_ESCAPE == nChar) // key enter
	{
	}
	else if (VK_TAB == nChar) // key tab
	{
		for (int i=0;i<8;++i) InsertChar(L' ');	// insert 7 space
	}
	else if (VK_BACK == nChar) // key back space
	{
		DeleteChar(DP_LEFT);
	}
	else if (VK_DELETE == nChar) // key delete
	{
		DeleteChar(DP_RIGHT);
	}
	else if (VK_HOME == nChar)
	{
		m_nCharPos = 0;
	}
	else if (VK_END == nChar)
	{
		m_nCharPos = (UINT)wcslen(m_sCookie);
	}
	else if (VK_LEFT == nChar) // key left
	{
		if (m_nCharPos > 0)
		{
			--m_nCharPos;
		}
	}
	else if (VK_RIGHT == nChar) // key right
	{
		if (m_nCharPos < wcslen(m_sCookie))
		{
			++m_nCharPos;
		}
	}
	else
	{
		if (m_nCharPos < COOKIE_SIZE)
		{
			char szImeChar[3];

			if (nChar < 128)
			{
				szImeChar[0]=(char)nChar;   
				szImeChar[1]='\0';   
			}
			else
			{
				szImeChar[0]=(char)(nChar>>8);   
				szImeChar[1]=(char)nChar;
				szImeChar[2]='\0';
			}

			InsertChar(*CA2W(szImeChar));
		}
	}
}

void GfxEdit::InsertChar(wchar_t aChar)
{
	UINT nLen = (UINT)wcslen(m_sCookie);
	if (m_nCharPos < nLen)
	{
		while(m_nCharPos < nLen)
		{
			m_sCookie[nLen] = m_sCookie[nLen-1];
			--nLen;
		}
	}
	m_sCookie[m_nCharPos++] = aChar;
}

void GfxEdit::DeleteChar(TDP aPos)
{
	if (DP_LEFT == aPos)
	{
		if (m_nCharPos == 0)
		{
			return;
		}
		--m_nCharPos;
	}
	else if (DP_RIGHT == aPos)
	{
		if (m_nCharPos == (UINT)wcslen(m_sCookie))
		{
			return;
		}
	}
	else
	{
		return;
	}

	UINT nIndex = m_nCharPos;
	while(m_sCookie[nIndex] != 0)
	{
		m_sCookie[nIndex] = m_sCookie[nIndex+1];
		nIndex++;
	}
}

void GfxEdit::SetCallback( pGfxEditBackFun pFunc )
{
	pBackFunc = pFunc;
}
