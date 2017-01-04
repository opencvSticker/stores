#pragma once

#include "../common/Line.h"
#include "../common/common.h"
#include "../bmp/BMP.h"

namespace stickerEditor
{
	class StickerRow;
	
	struct LineInShow 
	{
		virtual RowType type() = 0;
		virtual int getLineHeight() = 0;
		virtual int getCursorLen()
		{
			return 20;
		}
		virtual void show(HDC &hDCMem, shared_ptr<LineInShow> pLineInShow, const int beginHeight) = 0;

		shared_ptr<StickerRow> stickerRowRef;
	};

	struct PicInShow:public LineInShow 
	{
		RowType type();
		int getLineHeight();
		void show(HDC &hDCMem, shared_ptr<LineInShow> pLineInShow, const int beginHeight);		

		shared_ptr<ToBmpFormat> m_picInfo;

	private:
		LRESULT warnInfo(HDC &hDCMem, const int beginHeight);
	};

	struct EmptyInShow:public LineInShow
	{
		RowType type();
		int getLineHeight();
		void show(HDC &hDCMem, shared_ptr<LineInShow> pLineInShow, const int beginHeight);
	};

	struct textLineInfo
	{
		unsigned int everyShowBeginPos;
		unsigned int maxHeight;
		
	};

	struct TextLineInShow:public LineInShow
	{
		RowType type();
		int getLineHeight();
		int getCursorLen();
		void show(HDC &hDCMem, shared_ptr<LineInShow> pLineInShow, const int beginHeight);

		shared_ptr<Line> lineRef;
		vector<textLineInfo> lineInfoVec;
	};
	
	void LineSplit2Show(RECT region, shared_ptr<Line> &lineRef, TextLineInShow * lineInShow);
}