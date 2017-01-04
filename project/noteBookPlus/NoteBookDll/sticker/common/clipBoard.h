#pragma once

#include <windows.h>
#include <vector>
#include "../bmp/bmp.h"

using namespace std;

namespace stickerEditor
{

class ClipBoard
{
public:
	static bool getClipBoardText(HWND hWnd,vector<wchar_t> &wchs);
	static bool getClipBoardBitmap(HWND hWnd, ToBmpFormat * picInfo);
};

}