//----------------------------------------------------------------------------
// Copyright 2021, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Rect.h"

namespace Azul
{
	Rect::Rect(float _x, float _y, float _width, float _height)
		: x(_x), y(_y), width(_width), height(_height)
	{

	}

	Rect::Rect()
		: x(0.0f), y(0.0f), width(0.0f), height(0.0f)
	{

	}

	void Rect::clear()
	{
		this->x = 0.0f;
		this->y = 0.0f;
		this->height = 0.0f;
		this->width = 0.0f;
	}
}

// --- End of File ---
