#pragma once
#include "common/common.h"

namespace stickerEditor
{
class Sticker
{
public:
	static bool regWndClass(HINSTANCE hIns);
	static bool unRegWndClass(HINSTANCE hIns);
};

}

