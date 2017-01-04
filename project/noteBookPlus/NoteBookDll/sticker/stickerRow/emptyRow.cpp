#include "emptyRow.h"

namespace stickerEditor
{
	EmptyRow::~EmptyRow()
	{
	}
	RowType EmptyRow::type()
	{
		return empty_row;
	}

	void EmptyRow::splitToShow( RECT region, shared_ptr<LineInShow> &lineInShow)
	{
		if (lineInShow == NULL)
		{
			lineInShow = make_shared<EmptyInShow>();
		}
	}

	POINT EmptyRow::getCursorPos(RECT region, unsigned int curPos)
	{
		POINT point;
		point.x = region.left + 1;
		point.y = 1;
		return point;
	}

	unsigned int EmptyRow::onVKeyLeft(unsigned int curInsertPos)
	{
		return 0;
	}
	unsigned int EmptyRow::onVKeyRight(unsigned int curInsertPos)
	{
		return 0;
	}

	void EmptyRow::connectTwoRows(shared_ptr<StickerRow> row, unsigned int &curPos)
	{

	}
}