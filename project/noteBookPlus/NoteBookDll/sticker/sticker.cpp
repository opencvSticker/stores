#include "sticker.h"
#include "stickerWndClass.h"

namespace stickerEditor
{

	bool Sticker::regWndClass(HINSTANCE hIns)
    {
        return StickerWndClass::reg(hIns);
    }

	bool Sticker::unRegWndClass(HINSTANCE hIns)
	{
		return StickerWndClass::unReg(hIns);
	}

}
