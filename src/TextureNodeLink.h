//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef TEXTURE_NODE_LINK_H
#define TEXTURE_NODE_LINK_H

namespace Azul
{
	class TextureNodeLink
	{
	public:
		TextureNodeLink()
		{
			this->next = 0;
			this->prev = 0;
		}

		TextureNodeLink* next;
		TextureNodeLink* prev;
	};

}
#endif

// --- End of File --------------------------------------------------
