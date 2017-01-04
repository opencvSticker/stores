#pragma  once

#include "perChar.h"

#include <windows.h>
#include <vector>
#include <list>
#include <memory>
using namespace std;

namespace stickerEditor
{
	struct LineUnit
	{
		shared_ptr<PerChar> data[LINE_INC_SIZE];

		LineUnit()
		{
			for (int i=0;i <LINE_INC_SIZE; i++)
			{
				data[i] = make_shared<PerChar>();
			}
		}

	};


class Line
{
public:
	Line();
	unsigned int getLineFlag();
	unsigned int size() const;
	void append(WCHAR wch, shared_ptr<SFont>  font);
	void append(PerChar& wch);
	void addWCh(unsigned int pos,WCHAR wch, shared_ptr<SFont>  font);
	shared_ptr<PerChar> getPerChar(unsigned int pos);
	void removePerChar(unsigned int pos);
	void setLineLength(unsigned int lineLen);

private:
	void wchar2PerChar(PerChar *pc,WCHAR wch, shared_ptr<SFont>  font);
	shared_ptr<PerChar> setPerChar(unsigned int pos, PerChar & pc);	
	
private:
	std::vector<shared_ptr<LineUnit>> m_data;
	unsigned int m_lineLen;
	unsigned int m_lineFlag;

	static unsigned int lineFlag;

};


}