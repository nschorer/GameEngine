#ifndef ANIM_CONTROLLER_LINK_H
#define ANIM_CONTROLLER_LINK_H

namespace Azul
{
	class AnimControllerLink
	{
	public:
		AnimControllerLink()
		{
			this->next = nullptr;
			this->prev = nullptr;
		}

		virtual ~AnimControllerLink()
		{

		}

		AnimControllerLink* next;
		AnimControllerLink* prev;
	};
}
#endif