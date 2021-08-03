//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef TIMER_CONTROLLER_H
#define TIMER_CONTROLLER_H

#include "AnimTimer.h"

namespace Azul
{
	class TimerController
	{
	public:
		TimerController(AnimTime Delta, AnimTime Min, AnimTime Max);

		TimerController() = delete;
		TimerController(const TimerController &) = delete;
		TimerController &operator = (const TimerController &) = delete;
		~TimerController() = default;

		void Update();

		// Accessors
		void SetCurrTime(const AnimTime time);
		AnimTime GetCurrTime() const;
		void SetDeltaTime(const AnimTime time);
		AnimTime GetDeltaTime() const;
		void SetMaxTime(const AnimTime time);
		AnimTime GetMaxTime() const;
		void SetMinTime(const AnimTime time);
		AnimTime GetMinTime() const;

	private:
		AnimTime tCurr;
		AnimTime tDelta;
		AnimTime tMax;
		AnimTime tMin;
	};
}

#endif

// --- End of File ---

