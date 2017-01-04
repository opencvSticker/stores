#pragma once
#include "stickerRow.h"

namespace stickerEditor
{
	class EmptyRow :public StickerRow
	{
	public:
		RowType type();
		~EmptyRow();
		void splitToShow(RECT region, shared_ptr<LineInShow>& lineInShow);
		POINT getCursorPos(RECT region, unsigned int curPos);
		void connectTwoRows(shared_ptr<StickerRow> row, unsigned int &curPos);
		unsigned int onVKeyLeft(unsigned int curInsertPos);
		unsigned int onVKeyRight(unsigned int curInsertPos);

	};
}