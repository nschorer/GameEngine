//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef ANIM_CONTROLLER_H
#define ANIM_CONTROLLER_H

#include "TimerController.h"
#include "GameObjectAnim.h"
#include "Anim.h"
#include "AnimControllerLink.h"

namespace Azul
{
	class AnimController : public AnimControllerLink
	{
	public:

		enum class Name
		{
			Test,
			Humanoid,
			Teddy
		};

		AnimController(const AnimController &) = delete;
		AnimController &operator = (const AnimController &) = delete;
		~AnimController();

		AnimController(Name name, const char* const pAnimFileName, AnimTime delta = 0.1f * AnimTime(AnimTime::Duration::NTSC_30_FRAME), CustomModel* pModel = nullptr);
		AnimController(Name name, Clip* pClip, AnimTime delta = 0.1f * AnimTime(AnimTime::Duration::NTSC_30_FRAME));

		void SpeedUp();
		void SlowDown();

		void Update();

		void SetClip(Clip* pClip);
		void TransitionToClip(Clip* pClip);

		Name GetName();

	private:
		TimerController *poTimerControl;
		Anim            *poAnim;
		Name name;
	};
}

#endif

// --- End of File ---
