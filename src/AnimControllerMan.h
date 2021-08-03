#ifndef ANIM_CONTROLLER_MAN_H
#define ANIM_CONTROLLER_MAN_H

#include "AnimController.h"

namespace Azul
{

	// Singleton
	class AnimControllerMan
	{
	public:
		static AnimController* Add(AnimController::Name name, const char* const filename, CustomModel* pModel = nullptr);
		static AnimController* Add(AnimController::Name name, Clip* pClip);
		static AnimController* Find(AnimController::Name name);
		static void Update();

		static void Remove(AnimController::Name name);

		static void Destroy();
		static void Create();


		~AnimControllerMan() = default;
		AnimControllerMan(const AnimControllerMan&) = delete;
		AnimControllerMan& operator=(const AnimControllerMan&) = delete;



	private:
		AnimControllerMan();
		static AnimControllerMan* privGetInstance();

		void privAddToFront(AnimControllerLink* node, AnimControllerLink*& head);
		void privRemove(AnimControllerLink* pNode, AnimControllerLink*& poHead);

	private:
		AnimControllerLink* active;
	};

}

#endif