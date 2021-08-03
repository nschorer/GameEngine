#ifndef CLIP_LINK_H
#define CLIP_LINK_H

namespace Azul
{
	class ClipLink
	{
	public:
		ClipLink()
		{
			this->next = nullptr;
			this->prev = nullptr;
		}

		virtual ~ClipLink()
		{

		}

		ClipLink* next;
		ClipLink* prev;
	};
}
#endif