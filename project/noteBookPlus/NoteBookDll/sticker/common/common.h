#pragma once

#include <windows.h>
#include <tchar.h>

namespace stickerEditor
{
#define wndClassName _T("__stickerWndClassName__")
#define LineSpacing (2)
#define CharSpacing (1)

#define ID_TIMER (1011)
#define CURSOR_BLINKER_ELAPSE (500)

	enum RowType
	{
		empty_row,
		text_row,
		pic_row
    };

	struct PicInfo 
	{
		PicInfo():m_bits(NULL)
		{}
		~PicInfo()
		{
			if (m_bits!=NULL)
			{
				delete[] m_bits;
				m_bits = NULL;
			}
		}

		BITMAPINFO m_bmi;
		LPBYTE m_bits;
	};

#define WM_CHILD_SIZE (WM_USER + 1)
#define WM_POST_END (WM_CHILD_SIZE)

}
