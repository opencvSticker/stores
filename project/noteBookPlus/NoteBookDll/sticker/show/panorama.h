#pragma once

#include "../common/cursorPos.h"
#include "LineShow.h"
#include "../stickerRow/stickerRow.h"
#include "../stickerRow/textRow.h"

#include <list>
#include <map>
#include <memory>



using namespace std;


namespace stickerEditor
{
	struct ShowRegion
	{
		ShowRegion(LONG l, LONG t, LONG r, LONG b);
		ShowRegion();
		ShowRegion(RECT rc);

		RECT getRect();

		LONG    left;
		LONG    top;
		LONG    right;
		LONG    bottom;
	};


	class Panorama
	{
	public:
		Panorama(HWND hWnd,ShowRegion region = ShowRegion(-1,-1,-1,-1));
		ShowRegion setShowRegion(ShowRegion region);

		CursorPos getCursorPos(const list<shared_ptr<StickerRow>> &rows, list<shared_ptr<StickerRow>>::iterator &iter, unsigned int pos);

		void onPaint(const list<shared_ptr<StickerRow>> &rows, list<shared_ptr<StickerRow>>::iterator &iter, unsigned int pos);
		
		void show(const list<shared_ptr<StickerRow>> &rows, list<shared_ptr<StickerRow>>::iterator &iter, unsigned int pos);

	private:
		void initPanorama(const list<shared_ptr<StickerRow>> &rows, list<shared_ptr<StickerRow>>::iterator &iter, unsigned int pos);
		void showPanorama(HDC hDCMem);
	private:
		HWND m_hWnd;
		ShowRegion m_showRegion;

		list<shared_ptr<LineInShow>> m_showLines;


	};
}