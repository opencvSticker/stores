#pragma once

#include "common.h"

namespace stickerEditor
{
	struct CursorPos
	{
		POINT cursorPos_;
		int cursorLen_;

		CursorPos()
		{
			memset(this, 0, sizeof CursorPos);
		}

	};

}

