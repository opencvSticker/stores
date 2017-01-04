#pragma once
#include "stickerRow.h"
#include "../bmp/bmp.h"

namespace stickerEditor
{
	class PicRow :public StickerRow
	{
	public:
		RowType type();
		~PicRow();
		void splitToShow(RECT region, shared_ptr<LineInShow>& lineInShow);
		POINT getCursorPos(RECT region, unsigned int curPos);
		unsigned int onVKeyLeft(unsigned int curInsertPos);
		unsigned int onVKeyRight(unsigned int curInsertPos);

	public:
		void addPic(shared_ptr<ToBmpFormat> pic);

	private:
		shared_ptr<ToBmpFormat> m_picInfo;

	};
}