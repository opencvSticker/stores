#include "textRow.h"

namespace stickerEditor
{
	TextRow::TextRow():m_line(new Line())
	{

	}

	RowType TextRow::type()
	{
		return text_row;
	}


	void TextRow::addChar(unsigned int pos, WCHAR wch, shared_ptr<SFont>  font)
	{
		m_line->addWCh(pos, wch,font);
	}

	shared_ptr<Line> TextRow::getLine()
	{
		return m_line;
	}

	void TextRow::splitToShow(RECT region, shared_ptr<LineInShow> &lineInShow)
	{
		if (lineInShow == NULL)
		{
			lineInShow = make_shared<TextLineInShow>();
		}
		TextLineInShow *p = static_cast<TextLineInShow*>(lineInShow.get());
		LineSplit2Show(region, m_line, p);
	}

	POINT TextRow::getCursorPos(RECT region, unsigned int curPos)
	{
		POINT point;

		if (curPos > m_line->size())
		{
			curPos = m_line->size();
		}
		if (curPos == 0)
		{
			point.x = 1;
			point.y = 0;

			return point;
		}
		shared_ptr<PerChar> pc = m_line->getPerChar(curPos - 1);
		point.x = pc->posInShowLine.x + pc->getSize().cx + 1;
		point.y = pc->posInShowLine.y;

		return point;
	}

	void TextRow::connectTwoRows(TextRow* row, unsigned int &curPos)
	{
		if (row == NULL)
		{
			return;
		}

		curPos = m_line->size() + 1;

		for (int i = 0; i < row->m_line->size(); i++)
		{
			m_line->append(*(row->getLine()->getPerChar(i)));
		}

	}

	void TextRow::connectTwoRows(shared_ptr<StickerRow> row,unsigned int &curPos)
	{
		if (row->type() != text_row)
		{
			return;
		}

		connectTwoRows((TextRow*)row.get(), curPos);
		
	}

	unsigned int TextRow::onVKeyDel(unsigned int curInsertPos)
	{
		if (m_line->size() == 0)
		{
			return 0;
		}

		m_line->removePerChar(curInsertPos );

		return curInsertPos;
	}

	unsigned int TextRow::onVKeyBackspace(unsigned int curInsertPos)
	{
		if (m_line->size() == 0)
		{
			return 0;
		}

		if (curInsertPos > m_line->size() )
		{
			curInsertPos = m_line->size();
		}

		m_line->removePerChar(curInsertPos - 1);

		return curInsertPos - 1;

	}

	unsigned int TextRow::onVKeyLeft(unsigned int curInsertPos)
	{
		if (curInsertPos > m_line->size() && m_line->size() > 0)
		{
			return m_line->size() ;
		}

		if (curInsertPos > 0)
		{
			return curInsertPos - 1;
		}

		return 0;
	}
	unsigned int TextRow::onVKeyRight(unsigned int curInsertPos)
	{
		return 0;
	}

	bool TextRow::isEndOfRow(unsigned int curInsertPos)
	{
		if (curInsertPos > m_line->size())
		{
			return true;
		}
		return false;
	}

}