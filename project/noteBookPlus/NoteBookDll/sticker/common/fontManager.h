#pragma once

#include <windows.h>

#include <memory>
#include <map>

using namespace std;

namespace stickerEditor
{
	enum euFont
	{
		SONG_TI,   //宋体
		SONG_TI_BOLD,//宋体粗体
	};

	enum euCharFontSize
	{
		SONG_TO_SANHAO,// 宋体三号
		SONG_TO_XIAOSI//宋体小四
	};

	enum euCharColor
	{
		RED,
		BLUE,
		BLACK
	};

	enum SupportedFont
	{
		
		ST_XiaoSi_Black,
		ST_XiaoSi_Red,
		ST_XiaoSi_Blue,

		ST_SanHao_Black,
		ST_SanHao_Red,
		ST_SanHao_Blue,

		//the last is default
		SF_DEFAULT = ST_XiaoSi_Black
		
	};

class FontHelper;

class SFont
{
private:
	SFont(SupportedFont);	

public:
	~SFont();

	SupportedFont getFont();
	HFONT getHFont();
	SIZE getSize();
	COLORREF getColor();

private:
	HFONT createHFont();
private:

	SupportedFont m_sf;
	HFONT m_hFont;

friend class SFontFactory;
};

class FontHelper
{
public:
	static SIZE getFontSize(SupportedFont sf);
	static COLORREF getFontColor(SupportedFont sf);

private:
};

class SFontFactory
{
public:
	static shared_ptr<SFont> getSFont(SupportedFont sf) ;

private:
	static map<SupportedFont, shared_ptr<SFont>> m_existedSFont;
};
}