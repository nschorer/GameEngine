//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef SHADER_LINK_H
#define SHADER_LINK_H

namespace Azul
{
	class ShaderLink
	{
	public:
		ShaderLink()
		{
			this->next = nullptr;
			this->prev = nullptr;
		}

		virtual ~ShaderLink()
		{

		}

		ShaderLink* next;
		ShaderLink* prev;
	};
}

#endif

// ---  End of File ---
