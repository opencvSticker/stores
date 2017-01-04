#include "picRow.h"

namespace stickerEditor
{
	PicRow::~PicRow()
	{
	}
	RowType PicRow::type()
	{
		return pic_row;
	}

	void PicRow::splitToShow( RECT region, shared_ptr<LineInShow> &lineInShow)
	{
		if (lineInShow == NULL)
		{
			lineInShow = make_shared<PicInShow>();
		}

		if (lineInShow->type() == pic_row)
		{
			PicInShow *pPicInShow = (PicInShow*)lineInShow.get();
			pPicInShow->m_picInfo = m_picInfo;

		}
		
	}

	POINT PicRow::getCursorPos(RECT region, unsigned int curPos)
	{
		POINT point;
		point.x = region.left + 1;
		point.y = 1;
		return point;
	}

	unsigned int PicRow::onVKeyLeft(unsigned int curInsertPos)
	{
		return 0;
	}
	unsigned int PicRow::onVKeyRight(unsigned int curInsertPos)
	{
		return 0;
	}

	void PicRow::addPic(shared_ptr<ToBmpFormat> picInfo)
	{
		m_picInfo = picInfo;
	}
}