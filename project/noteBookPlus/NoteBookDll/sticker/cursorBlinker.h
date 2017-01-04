#pragma once
#include "common/common.h"
#include "common/cursorPos.h"

#include <map>

using namespace std;

namespace stickerEditor
{

	class CursorBlinker
	{
	public:
		CursorBlinker(HWND hWnd);
		~CursorBlinker();


		void draw(CursorPos *cursorPos, int cursorLen);

		//void setTimer(HWND hWnd);
		void clearCursor(HWND hWnd);

	private:
		void initPen();
		void drawCursor(CursorPos *cursorPos, int cursorLen, HWND hWnd, HPEN hPen);

	private:

		HWND m_hWnd;

		HPEN m_forwardPen;
		HPEN m_backwardPen;
		HPEN m_curPen;

		CursorPos m_cursorPos;

	};


}