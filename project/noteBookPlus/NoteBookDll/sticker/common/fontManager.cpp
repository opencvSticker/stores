#include "fontManager.h"

namespace stickerEditor
{

SFont::SFont(SupportedFont sf)
{
	m_sf = sf;	
	m_hFont = createHFont();
}

SFont::~SFont()
{
	if (m_hFont != NULL)
	{
		DeleteObject(m_hFont);
	}
}

SupportedFont SFont::getFont()
{
	return m_sf;
}

HFONT SFont::getHFont()
{
	if (m_hFont!= NULL)
	{
		return m_hFont;
	}
	
	return createHFont();
}

HFONT SFont::createHFont()
{
	LOGFONT   logfont;       //改变输出字体
	ZeroMemory(&logfont, sizeof(LOGFONT));
	logfont.lfCharSet = GB2312_CHARSET;
	logfont.lfHeight = FontHelper::getFontSize(m_sf).cy;      //设置字体的大小
	logfont.lfWidth = FontHelper::getFontSize(m_sf).cx;
	wsprintf(logfont.lfFaceName, TEXT("%s"), TEXT("宋体"));
	m_hFont = CreateFontIndirect(&logfont);

	return m_hFont;
}

SIZE SFont::getSize()
{
	return FontHelper::getFontSize(m_sf);
}

COLORREF SFont::getColor()
{
	return FontHelper::getFontColor(m_sf);
}

////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////


SIZE FontHelper::getFontSize(SupportedFont sf)
{
	SIZE sz;
	switch (sf)
	{
	case ST_SanHao_Black:
	case ST_SanHao_Blue:
	case ST_SanHao_Red:
		sz.cx = 20;
		sz.cy = 24;
		break;

	case ST_XiaoSi_Black:
	case ST_XiaoSi_Blue:
	case ST_XiaoSi_Red:
		sz.cx = 16;
		sz.cy = 20;
		break;

	default:
		sz.cx = 20;
		sz.cy = 24;
	}

	return sz;
}
COLORREF FontHelper::getFontColor(SupportedFont sf)
{
	COLORREF color;
	
	switch (sf)
	{
	case ST_SanHao_Black:
	case ST_XiaoSi_Black:
		color = RGB(0, 0, 0);
		break;

	case ST_SanHao_Blue:
	case ST_XiaoSi_Blue:
		color = RGB(0, 255, 0);
		break;
	case ST_SanHao_Red:
	case ST_XiaoSi_Red:
		color = RGB(255, 0, 0);
		break;

	default:
		color = RGB(0, 0, 0);
	}

	return color;

}


//////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
map<SupportedFont, shared_ptr<SFont>> SFontFactory::m_existedSFont;

shared_ptr<SFont> SFontFactory::getSFont(SupportedFont sf)
{
	map<SupportedFont, shared_ptr<SFont>>::iterator iter;
	iter = m_existedSFont.find(sf);
	if ( iter!= m_existedSFont.end())
	{
		return iter->second;
	}

	SFont * pSFont = new SFont(sf);
	shared_ptr<SFont> newSFont(pSFont);
	m_existedSFont.insert(make_pair(sf, newSFont));

	return newSFont;
}


}