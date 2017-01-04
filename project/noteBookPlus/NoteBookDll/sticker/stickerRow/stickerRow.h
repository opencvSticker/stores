#pragma once

#include "../common/common.h"
#include "../show/LineShow.h"
#include <memory>

using namespace std;

namespace stickerEditor
{	

	class StickerRow
	{
	public:
		virtual ~StickerRow(){}
		virtual RowType type() = 0;
		virtual POINT getCursorPos(RECT region, unsigned int curPos) = 0;
		virtual void connectTwoRows(shared_ptr<StickerRow> row,unsigned int &curPos){}
		virtual void splitToShow(RECT region, std::shared_ptr<LineInShow> &lineInShow) = 0;
		virtual bool isEndOfRow(unsigned int curInsertPos) {return true;}
		virtual unsigned int onVKeyLeft(unsigned int curInsertPos) = 0;
		virtual unsigned int onVKeyRight(unsigned int curInsertPos) = 0;
		virtual unsigned int onVKeyBackspace(unsigned int curInsertPos) { return 0; };
		virtual unsigned int onVKeyDel(unsigned int curInsertPos) { return 0; }

	};
}