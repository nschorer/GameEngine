//----------------------------------------------------------------------------
// Copyright 2021, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef IMAGE_LINK_H
#define IMAGE_LINK_H

namespace Azul
{
	class ImageLink
	{
	public:
		ImageLink()
		{
			this->next = nullptr;
			this->prev = nullptr;
		}

		virtual ~ImageLink()
		{

		}

		ImageLink *next;
		ImageLink *prev;
	};
}

#endif

//--- End of File ---
