#include "wndFocusManager.h"

namespace stickerEditor
{

	HWND WndFocusManager::m_lButtonDownHWnd = NULL;

	WndFocusManager::WndFocusManager() 
	{

	}

	void WndFocusManager::onMsg(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
	{
		if (iMessage == WM_LBUTTONDOWN )
		{
			m_lButtonDownHWnd = hWnd;
		}

		if (iMessage == WM_LBUTTONUP)
		{
			if (hWnd == m_lButtonDownHWnd)
			{
				::SetFocus(hWnd);
			}
		}
	}

}