#pragma once

#include <windows.h>
#include <memory>
#include "fontManager.h"

using namespace std;

namespace stickerEditor
{
#define LINE_INC_SIZE (1024)

#define COL_SPACING (1)
#define ROW_SPACING (1)

	

	
	struct PerChar
	{
		WCHAR wch;
		SupportedFont sf;		
		POINT posInShowLine;

		void setSize(SIZE sz)
		{
			setSize(sz.cx, sz.cy);
		}

		void setSize(LONG cx, LONG cy)
		{
			chSize.cx = cx;
			chSize.cy = cy;
		}

		SIZE getSize()
		{
			SIZE sz;
			if (wch>128)
			{
				sz.cx = chSize.cx * 2;
				sz.cy = chSize.cy;

				return sz;
			}

			return chSize;
		}

		PerChar()
		{
			chSize.cx = chSize.cy = 0;
		}

		PerChar(const PerChar &pc)
		{
			this->operator=(pc);
		}

		PerChar& operator=(const PerChar& src) 
		{ 
			wch = src.wch;
			sf = src.sf;
			posInShowLine = src.posInShowLine;
			chSize = src.chSize;

			return *this;
		}

	private:
		SIZE chSize;

	};

}
