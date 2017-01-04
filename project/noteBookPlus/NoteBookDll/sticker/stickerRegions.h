#pragma once

#include "common/common.h"
#include "common/fontManager.h"
#include "stickerRow/textRow.h"
#include "stickerRow/emptyRow.h"
#include "stickerRow/picRow.h"
#include "common/cursorPos.h"
#include "show/panorama.h"
#include "cursorBlinker.h"
#include <memory>
#include <list>
#include <map>
#include <boost/function.hpp>
#include <boost/bind.hpp>

using namespace std;

namespace stickerEditor
{
	class StickerRegions
	{
	public:
		StickerRegions(HWND hWnd);
		~StickerRegions();

		void drawCursor();
		HWND getHWnd();

		void insertPic();
		LRESULT onMsg(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
		

	private:
	    void initMsgMap();
	    void initVKeyMap();
		void initCtrlKeyMap();

		shared_ptr<CursorPos> getCursorPos();
		int getCursorLen();

		void setCurSFont(SupportedFont sf);

		LRESULT onPaint(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

		LRESULT onSize(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
		LRESULT onChildSize(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

		LRESULT onKeyDown(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
		LRESULT onVKeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam);
		LRESULT onVKeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam);
		LRESULT onVKeyLeft(HWND hWnd, WPARAM wParam, LPARAM lParam);
		LRESULT onVKeyRight(HWND hWnd, WPARAM wParam, LPARAM lParam);
		LRESULT onVKeyBackspace(HWND hWnd, WPARAM wParam, LPARAM lParam);
		LRESULT onVKeyReturn(HWND hWnd, WPARAM wParam, LPARAM lParam);
		LRESULT onVKeyDel(HWND hWnd, WPARAM wParam, LPARAM lParam);

		LRESULT onCtrlDown(HWND hWnd, WPARAM wParam, LPARAM lParam);
		LRESULT onKeyCtrlV(HWND hWnd, WPARAM wParam, LPARAM lParam);

		LRESULT onChar(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
		LRESULT onImeChar(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
		void storeChar(WPARAM wParam);
		void splitTextRow(unsigned int splitPos);

		void onVKControl();

		template<class T>
		void newRow(bool underCurrow = true);
		void eraseCurRow();

		void connectTwoRows(bool isToPreviousRow);
		void connectEmptyAndTextRows(EmptyRow * emptyRow, TextRow *textRow);
		void connectEmptyAndEmptyRows(EmptyRow * emptyRow, EmptyRow *textRow);
		void connectTextAndEmptyRows(TextRow * textRow, EmptyRow *  emptyRow);
		void connectTextAndTextRows(TextRow * textRow1, TextRow *  textRow2);

		template<class T>
		void replaceEmptyRow();

	private:
		HWND m_hWnd;

		CursorBlinker m_cursorBlinker;

		map<UINT,boost::function<LRESULT(HWND,UINT,WPARAM,LPARAM)>> m_msgMap;

		map<WPARAM,boost::function<LRESULT(HWND,WPARAM,LPARAM)>> m_vkeyMap;

		map<unsigned char, boost::function<LRESULT(HWND, WPARAM, LPARAM)>> m_ctrlkeyMap;

		list<shared_ptr<StickerRow>> m_rows;

		list<shared_ptr<StickerRow>>::iterator m_curRow;
		unsigned int m_curInserPos;
		shared_ptr<CursorPos> m_cursorPos;

		Panorama m_panorama;

		shared_ptr<SFont> m_curFont;
		

	};

	
}