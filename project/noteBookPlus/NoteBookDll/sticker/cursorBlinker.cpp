#include "cursorBlinker.h"

namespace stickerEditor
{
	CursorBlinker::CursorBlinker(HWND hWnd):m_hWnd(hWnd)
	{
		initPen();
	}
	CursorBlinker::~CursorBlinker()
	{
		DeleteObject(m_forwardPen);
		DeleteObject(m_backwardPen);
	}

	void CursorBlinker::initPen()
	{
		m_forwardPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		m_backwardPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));

		m_curPen == NULL;
	}
	
	void CursorBlinker::draw(CursorPos *cursorPos,int cursorLen)
	{
		if (cursorPos == NULL)
		{
			return;
		}

		m_cursorPos.cursorPos_.x = cursorPos->cursorPos_.x;
		m_cursorPos.cursorPos_.y = cursorPos->cursorPos_.y;
		
		static bool cursorFlag = true;

		HPEN pen;
		if (cursorFlag)
		{
			m_curPen = m_forwardPen;
		}
		else
		{
			m_curPen = m_backwardPen;
		}

		drawCursor(cursorPos,cursorLen, m_hWnd, m_curPen);

		cursorFlag = !cursorFlag;

	}

	void CursorBlinker::drawCursor(CursorPos *cursorPos,int cursorLen, HWND hWnd, HPEN hPen)
	{
		HDC hDC = ::GetDC(hWnd);

		HPEN old = (HPEN)SelectObject(hDC, hPen);

		MoveToEx(hDC, cursorPos->cursorPos_.x , cursorPos->cursorPos_.y, NULL);
		LineTo(hDC, cursorPos->cursorPos_.x, cursorPos->cursorPos_.y + cursorLen);

		SelectObject(hDC, old);

		::ReleaseDC(hWnd, hDC);
	}

	void CursorBlinker::clearCursor(HWND hWnd)
	{
	

	}

	//void CursorBlinker::setTimer(HWND hWnd)
	//{
	//	::SetTimer(hWnd, ID_TIMER, CURSOR_BLINKER_ELAPSE, 0);
	//}

}