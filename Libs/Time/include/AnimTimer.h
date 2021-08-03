//--------------------------------------------------------------
// Copyright 2021, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef ANIM_TIMER_H
#define ANIM_TIMER_H

/*
 * class AnimTimer
 * Measures real durations of time in terms of Time.
 *
 * The AnimTimer interface you see here owes itself to Matlab.
 * You only measure segements of time with a AnimTimer.  Use tic() to
 * mark the beginning of the segment.  Use toc() to mark the end.
 *
 * toc() always returns the amount of time that has elapsed since tic()
 * was called.  If you never call tic() on your AnimTimer then toc() will
 * return @link Time::Time() zero @endlink.
 *
 *
 * Example
 *
 *    #include "AnimTimer.h"
 *
 *    // Create a AnimTimer.
 *    AnimTimer timer;
 *
 *    // Mark our begin time.
 *    timer.tic();
 *
 *    // Do something for a while.
 *    ...
 *
 *    // Measure how much time has gone by since we called tic().
 *    const Time elapsedTime = timer.toc();
 *
 *
 */

 //---------------------------------------------------------------------------
 // HEADER FILES:
 //---------------------------------------------------------------------------

#include "AnimTime.h"

//---------------------------------------------------------------------------
// TEMPLATE DECLARATION:
//---------------------------------------------------------------------------

namespace Azul
{
	class AnimTimer
	{
	public:
		//-----------------------------------------------------------------
		// Constructors / Destructors
		//-----------------------------------------------------------------

		/* Creates a AnimTimer for the platform you are compiling on. */
		AnimTimer();
		AnimTimer(const AnimTimer &) = delete;
		AnimTimer &operator = (const AnimTimer &) = delete;
		~AnimTimer();

		//-----------------------------------------------------------------
		// Timing Methods
		//-----------------------------------------------------------------

		/*
		* Marks the moment in time that toc() will get measured against.
		* Call tic() to mark the beginning of the segment of time that
		* you want to measure.
		*/
		void Tic();

		/*
		* return: The duration of Time that has elapsed since tic() was
		* called.  Returns Time::Time() zero if tic() has
		* never been called on this AnimTimer.
		*/
		const AnimTime Toc() const;


	private:
		// static method
		static const AnimTime privGetSystemTime();

		// Data
		AnimTime privTicMark;
	};
}

#endif   // Timer_h

// ---  End of File ---
