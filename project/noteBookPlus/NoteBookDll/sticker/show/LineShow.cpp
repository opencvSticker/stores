#include "LineShow.h"

namespace stickerEditor
{

//----------------------------------------------------------------------------------------------------
	RowType PicInShow::type()
	{
		return pic_row;
	}

	int PicInShow::getLineHeight()
	{
		if (m_picInfo != NULL )
		{
			return m_picInfo->getBMPHeight();
		}
		
		return 30;
	}

	LRESULT PicInShow::warnInfo(HDC &hDCMem, const int beginHeight)
	{
		wchar_t msg[] = _T("this is picRow, need to do more, on it.");
		TextOut(hDCMem, 0, 0 + beginHeight, msg, lstrlen(msg));

		return 0;
	}
	
	void PicInShow::show(HDC &hDCMem, shared_ptr<LineInShow> pLineInShow, const int beginHeight)
	{
		if (m_picInfo == NULL)
		{
			warnInfo(hDCMem, beginHeight);
			return;
		}

		int width = m_picInfo->getBMPWidth();
		int height = m_picInfo->getBMPHeight();

		BITMAPINFOHEADER bmih = m_picInfo->getBitMapInfoHeader();
		BITMAPINFO bi;
		bi.bmiHeader = bmih;
		StretchDIBits(hDCMem, 0, beginHeight +2, width, height , 0, 0, width, height,
			m_picInfo->getBits(), &bi, DIB_RGB_COLORS, SRCCOPY);


		return;
		///////////////////////
		
		int x = 0;
		int y = 0;

		

		for (int j = 0; j < height; j++)
		{
			for (int i = 0; i < width; i++)
			{
				COLORREF rgb;
				rgb = m_picInfo->getPixel((height - j - 1)*width + (i + 1));

				::SetPixel(hDCMem, i, j, rgb);
			}
		}

				
	}

//----------------------------------------------------------------------------------------------------
	RowType EmptyInShow::type()
	{
		return empty_row;
	}

	int EmptyInShow::getLineHeight()
	{
		return 16;
	}

	void EmptyInShow::show(HDC &hDCMem, shared_ptr<LineInShow> pLineInShow, const int beginHeight)
	{
		

	}

//----------------------------------------------------------------------------------------------------
	RowType TextLineInShow::type()
	{
		return text_row;
	}

	int TextLineInShow::getLineHeight()
	{
		int height = 0;
		
		vector<textLineInfo>::iterator iter;
		for (iter = lineInfoVec.begin(); iter != lineInfoVec.end(); iter++)
		{
			height += iter->maxHeight;
			height += LineSpacing;
		}

		return height;

	}

	int TextLineInShow::getCursorLen()
	{

		return 0;

	}

	void TextLineInShow::show(HDC &hDCMem, shared_ptr<LineInShow> pLineInShow, const int beginHeight)
	{
		if (pLineInShow == NULL)
		{
			return;
		}


		HFONT hFont = NULL;		
		COLORREF color = 0;

		TextLineInShow *pTextLineInShow = static_cast<TextLineInShow*>(pLineInShow.get());

		unsigned int width = 0;
		unsigned int height = beginHeight;
		vector<textLineInfo>::iterator iter = pTextLineInShow->lineInfoVec.begin();
		vector<textLineInfo>::iterator flagIter;

		wchar_t addr[2];
		addr[1] = 0;
		for (int i = 0; i < pTextLineInShow->lineRef->size(); i++)
		{
			shared_ptr<PerChar> perChar = pTextLineInShow->lineRef->getPerChar(i);

			;
			if (hFont != SFontFactory::getSFont(perChar->sf)->getHFont())
			{
				hFont = SFontFactory::getSFont(perChar->sf)->getHFont();
				SelectObject(hDCMem, hFont);
			}		

			if (color != SFontFactory::getSFont(perChar->sf)->getColor())
			{
				color = SFontFactory::getSFont(perChar->sf)->getColor();
				::SetTextColor(hDCMem, SFontFactory::getSFont(perChar->sf)->getColor());
			}		
			
			addr[0] = perChar->wch;			
			TextOut(hDCMem, perChar->posInShowLine.x, perChar->posInShowLine.y + beginHeight, addr, 1);
		}
	}


	void LineSplit2Show(RECT region, shared_ptr<Line> &lineRef, TextLineInShow *lineInShow)
	{
		textLineInfo lineInfo;
		lineInfo.everyShowBeginPos = 0;
		lineInfo.maxHeight = 0;

		lineInShow->lineRef = lineRef;
		lineInShow->lineInfoVec.push_back(lineInfo);		

		int i = 0;
		int width = region.left;
		int height = 0;
		shared_ptr<PerChar> pc;
		wchar_t addr[2];
		for (; i < lineRef->size(); i++)
		{
			pc = lineRef->getPerChar(i);
			addr[0] = pc->wch;
			addr[1] = 0;
			
			pc->setSize(SFontFactory::getSFont(pc->sf)->getSize());

			if (lineInShow->lineInfoVec[lineInShow->lineInfoVec.size()-1].maxHeight < pc->getSize().cy )
			{
				lineInShow->lineInfoVec[lineInShow->lineInfoVec.size() - 1].maxHeight = pc->getSize().cy;
				lineInfo.maxHeight = pc->getSize().cy;
			}

			pc->posInShowLine.x = width;
			pc->posInShowLine.y = height;

			width += pc->getSize().cx + CharSpacing;
			if (width > (region.right - region.left))
			{
				width = region.left;
				height = height + lineInfo.maxHeight + LineSpacing;
				
				pc->posInShowLine.x = width;				
				pc->posInShowLine.y = height;				
								
				lineInfo.everyShowBeginPos = i;
				lineInfo.maxHeight = 0;
				lineInShow->lineInfoVec.push_back(lineInfo);
				width += pc->getSize().cx + CharSpacing;
			}
		}
	}
}