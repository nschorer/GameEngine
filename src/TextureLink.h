//----------------------------------------------------------------------------
// Copyright 2021, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef TEXTURE_LINK_H
#define TEXTURE_LINK_H

namespace Azul
{
	class TextureLink
	{
	public:
		TextureLink()
		{
			this->next = nullptr;
			this->prev = nullptr;
		}

		virtual ~TextureLink()
		{

		}

		TextureLink *next;
		TextureLink *prev;
	};
}
#endif

// --- End of File ---
