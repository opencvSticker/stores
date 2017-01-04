#pragma once
#include <windows.h>

class UnicodeLib
{
public:
	static void UTF_8ToUnicode(WCHAR* pOut, char *pText);
	//GBK to UTF-8
	static char* G2U(const char* gb2312);
};