#pragma  once

#include "common/common.h"
#include <map>

using namespace std;

namespace stickerEditor
{
	class WndFocusManager
	{
	public:
		WndFocusManager();
		static void onMsg(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

	private:
		static HWND m_lButtonDownHWnd;
	};
}