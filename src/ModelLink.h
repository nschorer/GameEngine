//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef MODEL_LINK_H
#define MODEL_LINK_H

namespace Azul
{
	class ModelLink
	{
	public:
		ModelLink()
		{
			this->next = nullptr;
			this->prev = nullptr;
		}

		virtual ~ModelLink()
		{

		}

		ModelLink* next;
		ModelLink* prev;
	};

}

#endif

// ---  End of File ---
