#include "stickerRegions.h"
#include "common/unicodeLib.h"
#include "common/clipBoard.h"
#include "bmp/BMP.h"

#pragma comment(lib,"imm32.lib")

namespace stickerEditor
{
	StickerRegions::StickerRegions(HWND hWnd):m_cursorPos(new CursorPos),m_panorama(hWnd),m_hWnd(hWnd),m_cursorBlinker(hWnd)
	{
		m_cursorPos->cursorPos_.x = m_cursorPos->cursorPos_.y = 1;
		
		m_curInserPos = 1;
		m_rows.push_back(make_shared<EmptyRow>());
		m_curRow = m_rows.begin();

		m_curFont = SFontFactory::getSFont(SF_DEFAULT);

		initMsgMap();
		initVKeyMap();
		initCtrlKeyMap();

		//testOnInit();
	}

	void StickerRegions::initMsgMap()
	{
		m_msgMap[WM_CHAR] = boost::bind(&StickerRegions::onChar, this, _1, _2, _3, _4);
		m_msgMap[WM_PAINT] = boost::bind(&StickerRegions::onPaint, this, _1, _2, _3, _4);
		m_msgMap[WM_KEYDOWN] = boost::bind(&StickerRegions::onKeyDown, this, _1, _2, _3, _4);
		m_msgMap[WM_SIZE] = boost::bind(&StickerRegions::onSize, this, _1, _2, _3, _4);
		m_msgMap[WM_CHILD_SIZE] = boost::bind(&StickerRegions::onChildSize, this, _1, _2, _3, _4);
		m_msgMap[WM_IME_CHAR] = boost::bind(&StickerRegions::onImeChar, this, _1, _2, _3, _4);
	}

	void StickerRegions::initVKeyMap()
	{
	    m_vkeyMap[VK_UP] = boost::bind(&StickerRegions::onVKeyUp, this, _1, _2, _3);
	    m_vkeyMap[VK_DOWN] = boost::bind(&StickerRegions::onVKeyDown, this, _1, _2, _3);
	    m_vkeyMap[VK_LEFT] = boost::bind(&StickerRegions::onVKeyLeft, this, _1, _2, _3);
	    m_vkeyMap[VK_RIGHT] = boost::bind(&StickerRegions::onVKeyRight, this, _1, _2, _3);
		m_vkeyMap[VK_BACK] = boost::bind(&StickerRegions::onVKeyBackspace, this, _1, _2, _3);
		m_vkeyMap[VK_RETURN] = boost::bind(&StickerRegions::onVKeyReturn, this, _1, _2, _3);
		m_vkeyMap[VK_DELETE] = boost::bind(&StickerRegions::onVKeyDel, this, _1, _2, _3);
	}

	void StickerRegions::initCtrlKeyMap()
	{
		m_ctrlkeyMap[0x16] = boost::bind(&StickerRegions::onKeyCtrlV, this, _1, _2, _3);
		//m_ctrlkeyMap[0x16] it's ctrl+c
	}

	HWND StickerRegions::getHWnd()
	{
		return m_hWnd;
	}

	void StickerRegions::drawCursor()
	{
		shared_ptr<CursorPos> cursorPos = getCursorPos();
		int cursorLen = getCursorLen();
		m_cursorBlinker.draw(cursorPos.get(),cursorLen);
	}

	void StickerRegions::eraseCurRow()
	{
	    if(m_curRow == m_rows.begin())
	    {
	        m_curRow = m_rows.erase(m_curRow);
	        if(m_curRow == m_rows.end())
	            newRow<EmptyRow>();
	    }
	    else
	    {
	        list<shared_ptr<StickerRow>>::iterator temRow;
	        temRow = m_curRow;
	        temRow--;

	        m_curRow = m_rows.erase(m_curRow);
            if(m_curRow == m_rows.end())
                m_curRow = temRow;
	    }
	}

	void StickerRegions::connectEmptyAndTextRows(EmptyRow * pEmptyRow, TextRow *pTextRow)
	{
		list<shared_ptr<StickerRow>>::iterator temRow;
		temRow = m_curRow;

		temRow--;
		(*temRow) = (*m_curRow);

		m_curRow = m_rows.erase(m_curRow);
		m_curRow--;
	}

	void StickerRegions::connectEmptyAndEmptyRows(EmptyRow * pEmptyRow1, EmptyRow * pEmptyRow2)
	{
		m_curRow = m_rows.erase(m_curRow);
		if (m_curRow != m_rows.begin())
		{
			m_curRow--;
		}
	}

	void StickerRegions::connectTextAndEmptyRows(TextRow * pTextRow, EmptyRow * pEmptyRow)
	{
		m_curRow = m_rows.erase(m_curRow);
		if (m_curRow != m_rows.begin())
		{
			m_curRow--;
			m_curInserPos = pTextRow->getLine()->size() + 1;
		}
	}

	void StickerRegions::connectTextAndTextRows(TextRow * pTextRow1, TextRow * pTextRow2)
	{
		pTextRow1->connectTwoRows(pTextRow2, m_curInserPos);
		m_curRow = m_rows.erase(m_curRow);

		m_curRow--;
	}

	void StickerRegions::connectTwoRows(bool isToPreviousRow)
	{
		list<shared_ptr<StickerRow>>::iterator firstRow;
		list<shared_ptr<StickerRow>>::iterator secRow;

	    list<shared_ptr<StickerRow>>::iterator temRow;
	    temRow = m_curRow;

		TextRow * pTextRow = NULL;
		EmptyRow * pEmptyRow = NULL;

	    if(isToPreviousRow)
	    {
	        if(m_curRow == m_rows.begin())
            {
                return;
            }			

            temRow--;

			if ((*temRow)->type() == empty_row && (*m_curRow)->type() == text_row)
			{
				connectEmptyAndTextRows((EmptyRow*)(*temRow).get(), (TextRow*)(*m_curRow).get());
			}			
			else if ((*temRow)->type() == text_row && (*m_curRow)->type() == empty_row)
			{
				connectTextAndEmptyRows((TextRow*)(*temRow).get(), (EmptyRow*)(*m_curRow).get());
			}
			else if ((*temRow)->type() == text_row && (*m_curRow)->type() == text_row)
			{
				connectTextAndTextRows((TextRow*)(*temRow).get(), (TextRow*)(*m_curRow).get());
			}
			else if ((*temRow)->type() == empty_row && (*m_curRow)->type() == empty_row)
			{
				connectEmptyAndEmptyRows((EmptyRow*)(*temRow).get(), (EmptyRow*)(*m_curRow).get());
			}           

	    }

	}

	LRESULT StickerRegions::onVKeyBackspace(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		if(m_curInserPos == 1)
        {
            connectTwoRows(true);
            return 0;
        }
		m_curInserPos = (*m_curRow)->onVKeyBackspace(m_curInserPos-1) + 1;
		if (m_curInserPos == 0)
		{
			newRow<EmptyRow>(false);
		}

		return 0;
	}

	LRESULT StickerRegions::onVKeyLeft(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		m_curInserPos = (*m_curRow)->onVKeyLeft(m_curInserPos);
		if (m_curInserPos == 0)
		{
			if (m_curRow != m_rows.begin())
			{
				m_curRow--;
				if ((*m_curRow)->type() == text_row)
				{
					TextRow * pTextRow = (TextRow *)(*m_curRow).get();
					m_curInserPos = pTextRow->getLine()->size() + 1;
				}				
			}
			else
			{
				m_curInserPos = 1;
			}
		}	
				
		return 0;
	}

	LRESULT StickerRegions::onVKeyRight(HWND hWnd, WPARAM wParam, LPARAM lParam)
    {
		if ((*m_curRow)->isEndOfRow(m_curInserPos))
		{
			list<shared_ptr<StickerRow>>::iterator temRow;
			temRow = m_curRow;
			temRow++;
			if (temRow != m_rows.end())
			{
				m_curRow++;
				m_curInserPos = 1;
			}
			return 0;
		}

		if ((*m_curRow)->type() == text_row)
		{
			TextRow * pTextRow = (TextRow *)(*m_curRow).get();
			m_curInserPos = m_curInserPos + 1;
		}
        return 0;
    }

	

	StickerRegions::~StickerRegions()
	{
	}

	void StickerRegions::insertPic()
	{
		newRow<PicRow>();
	}

	shared_ptr<CursorPos> StickerRegions::getCursorPos()
	{
		CursorPos cursorPos = m_panorama.getCursorPos(m_rows, m_curRow, m_curInserPos);
		
		m_cursorPos->cursorPos_.x = cursorPos.cursorPos_.x;
		m_cursorPos->cursorPos_.y = cursorPos.cursorPos_.y;

		return m_cursorPos;
	}

	int StickerRegions::getCursorLen()
	{
		if (m_curFont != NULL)
		{
			return m_curFont->getSize().cy + 1;
		}

		return 16;		
	}


	template<class T>
	void StickerRegions::replaceEmptyRow()
	{
		*m_curRow = make_shared<T>();
	}

    template<class T>
	void StickerRegions::newRow(bool underCurRow /* = true */)
	{
		if (underCurRow)
		{
			m_curRow++;
			m_curRow = m_rows.insert(m_curRow, make_shared<T>());
			m_curInserPos = 1;			
		}
		else
		{
			m_curRow = m_rows.insert(m_curRow, make_shared<T>());
			m_curInserPos = 1;
		}        
	}

	void StickerRegions::onVKControl()
	{
	    newRow<EmptyRow>();
	}

	LRESULT StickerRegions::onVKeyDel(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		list<shared_ptr<StickerRow>>::iterator temRow;
		temRow = m_curRow;
		temRow++;
		if ((*m_curRow)->isEndOfRow(m_curInserPos))
		{		
			if (temRow != m_rows.end())
			{
				onVKeyRight(hWnd, wParam, lParam);
				onVKeyBackspace(hWnd, wParam, lParam);
			}						
		}
		else
		{
			onVKeyRight(hWnd, wParam, lParam);
			onVKeyBackspace(hWnd, wParam, lParam);
		}

		return 0;
	}

	void StickerRegions::splitTextRow(unsigned int splitPos)
	{
		if ((*m_curRow)->type() != text_row)
		{
			return;
		}

		list<shared_ptr<StickerRow>>::iterator temRow;

		newRow<TextRow>();

		temRow = m_curRow;
		temRow--;

		TextRow* pCurRow = (TextRow*)(*m_curRow).get();
		TextRow* pTxtRow = (TextRow*)(*temRow).get();
		shared_ptr<Line> pLine = pTxtRow->getLine();

		PerChar pc;
		for (int i = splitPos; i< pLine->size(); i++)
		{
			shared_ptr<PerChar> pPC = pLine->getPerChar(i);

			pc = *pPC;

			
			pCurRow->getLine()->append(pc);
		}

		pTxtRow->getLine()->setLineLength(splitPos );




	}

	LRESULT StickerRegions::onVKeyReturn(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		if ((*m_curRow)->isEndOfRow(m_curInserPos))
		{
			storeChar(VK_RETURN);
		}
		else if(m_curInserPos == 1)
		{
			shared_ptr<StickerRow> temRow;
			list<shared_ptr<StickerRow>>::iterator temIter;			
			storeChar(VK_RETURN);
			temIter = m_curRow;
			temIter--;
			temRow = (*m_curRow);
			(*m_curRow) = (*temIter);
			(*temIter) = temRow;			
		}
		else
		{
			if ((*m_curRow)->type() == text_row)
			{
				splitTextRow(m_curInserPos - 1);
			}			
		}

		return 0;
	}
	LRESULT StickerRegions::onVKeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
	    if (m_curRow!= m_rows.begin())
        {
        	m_curRow--;
        }
		return 0;
	}

	LRESULT StickerRegions::onVKeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
	    if (m_curRow != m_rows.end())
        {
        	list<shared_ptr<StickerRow>>::iterator temIter = m_curRow;

        	temIter++;
        	if (temIter != m_rows.end())
        	{
        		m_curRow++;
        	}
        }

		return 0;
	}

	LRESULT StickerRegions::onKeyDown(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
	{
		map<WPARAM,boost::function<LRESULT(HWND,WPARAM,LPARAM)>>::iterator iter;
	    iter = m_vkeyMap.find(wParam);
	    if(iter != m_vkeyMap.end())
	    {
	        LRESULT  re = iter->second(hWnd,wParam,lParam);
			if (re == 0)
			{
				m_panorama.show(m_rows, m_curRow, m_curInserPos);
			}
	    }

	    return 0;
	}

	LRESULT StickerRegions::onChildSize(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
	{
		MoveWindow(m_hWnd, 0, 0, LOWORD(lParam), HIWORD(lParam), false);
		return 0;
	}

	LRESULT StickerRegions::onSize(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
	{
		ShowRegion region;
		region.left = region.top = 0;
		region.right = LOWORD(lParam);
		region.bottom = HIWORD(lParam);

		m_panorama.setShowRegion(region);
		m_panorama.show(m_rows, m_curRow, m_curInserPos);
		return 0;
	}

	LRESULT StickerRegions::onPaint(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
	{   
		m_panorama.onPaint(m_rows, m_curRow, m_curInserPos);
		return 0;
	}

	LRESULT StickerRegions::onImeChar(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
	{
		char ch[10];
		memset(ch, 0, 10);
		ch[0] = HIBYTE(wParam);
		ch[1] = LOBYTE(wParam);

		TCHAR s_ch[6];
		char * t = UnicodeLib::G2U(ch);
		WCHAR pOut[4];
		memset(pOut, 0, 4);
		UnicodeLib::UTF_8ToUnicode(pOut, t);

		storeChar(pOut[0]);
		m_panorama.show(m_rows, m_curRow, m_curInserPos);

		HIMC m_hIMC = ::ImmGetContext(m_hWnd);
		HWND hWndIME = ::ImmGetDefaultIMEWnd(hWnd);
		
		COMPOSITIONFORM cf;

		COMPOSITIONFORM Composition;
		Composition.dwStyle = CFS_POINT;
		Composition.ptCurrentPos.x = 50;
		Composition.ptCurrentPos.y = 50;
		ImmSetCompositionWindow(m_hIMC, &Composition);

		return 1;
	}

	LRESULT StickerRegions::onCtrlDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		map<byte, boost::function<LRESULT(HWND, WPARAM, LPARAM)>>::iterator iter;
		iter = m_ctrlkeyMap.find(wParam);
		if (iter != m_ctrlkeyMap.end())
		{
			iter->second(hWnd, wParam, lParam);
		}
		return 0;
	}

	LRESULT StickerRegions::onKeyCtrlV(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		shared_ptr<ToBmpFormat> pic = make_shared<ToBmpFormat>();;
		vector<wchar_t> wchs;

		if (ClipBoard::getClipBoardBitmap(hWnd, pic.get()))
		{
			newRow<PicRow>();
			if ((*m_curRow)->type() != empty_row)
			{				
				if ((*m_curRow)->type() == pic_row)
				{
					PicRow *pr = (PicRow*)(*m_curRow).get();
					pr->addPic(pic);
				}
			}
		}
		else if (ClipBoard::getClipBoardText(hWnd, wchs))
		{
			vector<wchar_t>::iterator iter;
			iter = wchs.begin();
			for (; iter != wchs.end(); iter++)
			{
				storeChar(*iter);
			}
		}

		m_panorama.show(m_rows, m_curRow, m_curInserPos);

		return 0;
	}

	LRESULT StickerRegions::onChar(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
	{
		short ctrlState = GetKeyState(VK_CONTROL);
		if (ctrlState < 0)//ctrl key is donw
		{
			onCtrlDown(hWnd, wParam, lParam);
			return 0;
		}
		//check the ASCII code table
		if (wParam>31 && wParam<127)
		{			
			storeChar(wParam);
			m_panorama.show(m_rows, m_curRow, m_curInserPos);
		}	

		return 0;
	}

	void StickerRegions::storeChar(WPARAM wParam)
	{
		if (wParam == VK_RETURN)
		{
			onVKControl();
			return ;
		}
		
		if ((*m_curRow)->type() == RowType::pic_row)
		{
			newRow<EmptyRow>();
		}

		if ((*m_curRow)->type() == RowType::empty_row)
		{
			replaceEmptyRow<TextRow>();
		}

		if ((*m_curRow)->type() == RowType::text_row)
		{
			TextRow *pTextRow = static_cast<TextRow*>(m_curRow->get());
			pTextRow->addChar(m_curInserPos - 1, wParam,m_curFont);
			m_curInserPos++;
		}

	}

	void StickerRegions::setCurSFont(SupportedFont sf)
	{
		m_curFont = SFontFactory::getSFont(sf);
	}

	LRESULT StickerRegions::onMsg(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
	{		
		LRESULT re = 0;

		map<UINT, boost::function<LRESULT(HWND, UINT, WPARAM, LPARAM)>>::iterator iter;
		iter = m_msgMap.find(iMessage);
		if (iter != m_msgMap.end())
		{
			re = iter->second(hWnd,iMessage,wParam,lParam);			
		}

		return re;
	}
}