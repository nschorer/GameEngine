//--------------------------------------------------------------
// Copyright 2021, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "AnimTimer.h"

namespace Azul
{
	//---------------------------------------------------------------------------
	// CONSTRUCTORS / DESTRUCTORS / ASSIGNMENT:
	//---------------------------------------------------------------------------
	AnimTimer::AnimTimer() :
		privTicMark(AnimTime::Duration::MAX)
	{ }


	AnimTimer::~AnimTimer()
	{ }


	//---------------------------------------------------------------------------
	// TIMING METHODS:
	//---------------------------------------------------------------------------
	void AnimTimer::Tic()
	{
		this->privTicMark = AnimTimer::privGetSystemTime();
	}


	const AnimTime AnimTimer::Toc() const
	{
		AnimTime elapsedTime;

		// If tick has been called...
		if (AnimTime(AnimTime::Duration::MAX) != this->privTicMark)
		{
			elapsedTime = AnimTimer::privGetSystemTime() - this->privTicMark;
		}

		return(elapsedTime);
	}
}

// ---  End of File ---
