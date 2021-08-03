//----------------------------------------------------------------------------
// Copyright 2021, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef RECT_H
#define RECT_H

namespace Azul
{
	struct Rect
	{
		Rect(float x, float y, float width, float height);
		Rect();

		void clear();

		float x;
		float y;
		float width;
		float height;
	};
}

#endif

// --- End of File ---
