#pragma once
#include "stickerRow.h"
#include "../common/Line.h"

#include <memory>

using namespace std;

namespace stickerEditor
{
	class TextRow:public StickerRow
	{
	public:
		TextRow();
		RowType type();
		void splitToShow(RECT region, shared_ptr<LineInShow> &lineInShow);
		POINT getCursorPos(RECT region, unsigned int curPos);
		void connectTwoRows(shared_ptr<StickerRow> row, unsigned int &curPos);
		void connectTwoRows(TextRow* row, unsigned int &curPos);
		bool isEndOfRow(unsigned int curInsertPos);
		unsigned int onVKeyLeft(unsigned int curInsertPos);
		unsigned int onVKeyRight(unsigned int curInsertPos);
		unsigned int onVKeyBackspace(unsigned int curInsertPos);
		unsigned int onVKeyDel(unsigned int curInsertPos);

		void appendChar(wchar_t wch);
		void addChar(unsigned int pos, WCHAR wch, shared_ptr<SFont>  font);
		shared_ptr<Line> getLine();

	private:

	private:
		shared_ptr<Line> m_line;

	};
}