#include "Line.h"

namespace stickerEditor
{

unsigned int Line::lineFlag = 1;

Line::Line():m_lineLen(0)
{
	m_lineFlag = lineFlag;
	lineFlag++;
}

unsigned int Line::getLineFlag()
{
	return m_lineFlag;
}

unsigned int Line::size() const
{
	return m_lineLen;
}

void Line::wchar2PerChar(PerChar *pc,WCHAR wch, shared_ptr<SFont> font)
{
	pc->wch = wch;
	pc->sf = font->getFont();	
}

shared_ptr<PerChar> Line::setPerChar(unsigned int pos, PerChar & pc)
{
	shared_ptr<PerChar> originalPC = make_shared<PerChar>();

	unsigned int model = pos % LINE_INC_SIZE;
	unsigned int aliquot = pos / LINE_INC_SIZE;

	*originalPC = *m_data[aliquot]->data[model];

	(*m_data[aliquot]->data[model]) = pc;

	return originalPC;
}

shared_ptr<PerChar> Line::getPerChar(unsigned int pos)
{
	unsigned int model = pos % LINE_INC_SIZE;
	unsigned int aliquot = pos / LINE_INC_SIZE;

	return m_data[aliquot]->data[model];
}

void Line::addWCh(unsigned int pos,WCHAR wch, shared_ptr<SFont>  font)
{
	if ((pos+1)>m_lineLen)
	{
		append(wch,font);
		return;
	}

	unsigned int model = pos % LINE_INC_SIZE;
	unsigned int aliquot = pos / LINE_INC_SIZE;

	shared_ptr<PerChar> nextPC;

	unsigned int index = pos;
	unsigned int count = m_lineLen - pos ;

	nextPC = make_shared<PerChar>();
	wchar2PerChar(nextPC.get(),wch,font);

	for (int i = 0; i<count; i++)
	{
		nextPC = setPerChar(index + i , *nextPC);	
	}

	append(*nextPC);

}

void Line::append(PerChar& pc)
{
	unsigned int model = m_lineLen % LINE_INC_SIZE;
	unsigned int aliquot = m_lineLen / LINE_INC_SIZE;

	if (model == 0)
	{
		m_data.push_back(make_shared<LineUnit>());
	}

	*m_data[aliquot]->data[model] = pc;
	
	m_data[aliquot]->data[model];
	m_lineLen++;

}


void Line::append(WCHAR wch, shared_ptr<SFont>  font)
{
	unsigned int model = m_lineLen % LINE_INC_SIZE;
	unsigned int aliquot = m_lineLen / LINE_INC_SIZE;

	if (model == 0)
	{		
		m_data.push_back(make_shared<LineUnit>());
	}
	
	wchar2PerChar(m_data[aliquot]->data[model].get(),wch,font);
	m_lineLen++;
}

void Line::removePerChar(unsigned int pos)
{
	if (m_lineLen == 0)
	{
		return;
	}

	if ( (pos + 1) > m_lineLen)
	{
		m_lineLen--;
		return;
	}

	unsigned int begin = pos;
	unsigned int count = m_lineLen - pos + 1;

	shared_ptr<PerChar> nextChar = getPerChar(pos+1);
	for (int i = pos; i < m_lineLen; i++)
	{		
		shared_ptr<PerChar> nextChar = getPerChar(i + 1);
		setPerChar(i, *nextChar);
	}

	m_lineLen--;
}

void Line::setLineLength(unsigned int lineLen)
{
	if (lineLen > m_lineLen)
	{
		return;
	}
	m_lineLen = lineLen;
}

}