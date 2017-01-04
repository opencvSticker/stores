#include "panorama.h"

namespace stickerEditor
{
	ShowRegion::ShowRegion(LONG l, LONG t, LONG r, LONG b)
	{
		left = l;
		top = t;
		right = r;
		bottom = b;
	}

	ShowRegion::ShowRegion(){}

	ShowRegion::ShowRegion(RECT rc)
	{
		left = rc.left;
		top = rc.top;
		right = rc.right;
		bottom = rc.bottom;
	}

	RECT ShowRegion::getRect()
	{
		RECT rc;
		rc.left = left;
		rc.top = top;
		rc.right = right;
		rc.bottom = bottom;

		return rc;
	}
	
	Panorama::Panorama(HWND hWnd,ShowRegion region) :m_hWnd(hWnd)
	{
		if (region.left = -1)
		{
			RECT rc;
			::GetClientRect(m_hWnd, &rc);
			rc.left = rc.top = 1;
			rc.bottom -= 1;
			rc.right -= 1;
			setShowRegion(rc);
		}
		else
		{
			setShowRegion(region);
		}				
	}

	ShowRegion Panorama::setShowRegion(ShowRegion region)
	{
		ShowRegion oldRC = m_showRegion;
		m_showRegion = region;

		return oldRC;
	}

	void Panorama::showPanorama(HDC hDCMem)
	{
		int beginHeight = 0;
		list<shared_ptr<LineInShow>>::iterator iter;
		iter = m_showLines.begin();
		for (; iter!=m_showLines.end(); iter++)
		{
			(*iter)->show(hDCMem,*iter,beginHeight);
			beginHeight += (*iter)->getLineHeight();
		}
		
	}

	CursorPos Panorama::getCursorPos(const list<shared_ptr<StickerRow>> &rows, list<shared_ptr<StickerRow>>::iterator &iter, unsigned int pos)
	{		
		HDC hdc = NULL;
		int beginHeight = 0;
		list<shared_ptr<LineInShow>>::iterator linesIter;
		linesIter = m_showLines.begin();
		for (; linesIter != m_showLines.end(); linesIter++)
		{
			if (iter->get() == (*linesIter)->stickerRowRef.get())//the row cursor should be in found.
			{
				POINT point = (*iter)->getCursorPos(m_showRegion.getRect(),pos-1);

				CursorPos cursorPos;
				cursorPos.cursorPos_.x = point.x ;
				cursorPos.cursorPos_.y = point.y + beginHeight ;

				return cursorPos;
			}

			beginHeight += (*linesIter)->getLineHeight();	
			
		}

		return CursorPos();
	}

	void Panorama::onPaint(const list<shared_ptr<StickerRow>> &rows, list<shared_ptr<StickerRow>>::iterator &iter, unsigned int pos)
	{
		if (!m_showLines.empty())
		{
			initPanorama(rows, iter, pos);
		}

		PAINTSTRUCT ps;
		HDC hDC = ::BeginPaint(m_hWnd, &ps);

		int beginHeight = 0;
		list<shared_ptr<LineInShow>>::iterator linesIter;
		linesIter = m_showLines.begin();
		for (; linesIter != m_showLines.end(); linesIter++)
		{
			(*linesIter)->show(hDC, *linesIter, beginHeight);
			beginHeight += (*linesIter)->getLineHeight();
		}

		::EndPaint(m_hWnd, &ps);
	}

	void Panorama::show(const list<shared_ptr<StickerRow>> &rows, list<shared_ptr<StickerRow>>::iterator &iter, unsigned int pos)
	{		
		HDC hDC;
		RECT rect;

		GetClientRect(m_hWnd, &rect);
		hDC = ::GetDC(m_hWnd);


		HDC hDCMem = CreateCompatibleDC(hDC);
		HBITMAP hBmpMem = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
		SelectObject(hDCMem, hBmpMem);

		HBRUSH hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
		FillRect(hDCMem, &rect, hBrush);		
		
		initPanorama(rows, iter, pos);		
		showPanorama(hDCMem);	


		BitBlt(hDC, 0, 0, rect.right, rect.bottom, hDCMem, 0, 0, SRCCOPY);
		::DeleteDC(hDCMem);

		DeleteObject(hBrush);
		::ReleaseDC(m_hWnd, hDC);
		
	}

	void Panorama::initPanorama(const list<shared_ptr<StickerRow>> &rows, list<shared_ptr<StickerRow>>::iterator &iter, unsigned int pos)
	{
		list<shared_ptr<StickerRow>>::iterator forwardIter(iter);
		list<shared_ptr<StickerRow>>::iterator backwardIter(iter);

		shared_ptr<LineInShow> lineInShow;

		m_showLines.clear();

		int height = 0;
		do
		{
			lineInShow = NULL;
			(*forwardIter)->splitToShow(m_showRegion.getRect(), lineInShow);
			lineInShow->stickerRowRef = (*forwardIter);//this must be behind of splitToShow(...),
			                                           //because linkeInShow maybe null

			
			if ((height + lineInShow->getLineHeight()) > (m_showRegion.bottom - m_showRegion.top))
			{
				break;
			}

			m_showLines.push_front(lineInShow);
			height += lineInShow->getLineHeight();

			lineInShow = NULL;

			if (forwardIter==rows.begin())
			{
				break;
			}
			forwardIter--;
		} while (forwardIter != rows.end());
		
		backwardIter++;
		while (backwardIter !=rows.end())
		{
			lineInShow = NULL;
			(*backwardIter)->splitToShow(m_showRegion.getRect(), lineInShow);
			
			if ((height + lineInShow->getLineHeight()) > (m_showRegion.bottom - m_showRegion.top))
			{
				break;
			}

			m_showLines.push_back(lineInShow);
			height += lineInShow->getLineHeight();		

			backwardIter++;
		}
		

		

	}
}