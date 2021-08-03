//--------------------------------------------------------------
// Copyright 2021, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "AnimTimer.h"

//---------------------------------------------------------------------------
// NATIVE IMPLEMENTATION:
//---------------------------------------------------------------------------

namespace Azul
{
	const AnimTime AnimTimer::privGetSystemTime()
	{
		const static AnimTime oneSecond(AnimTime::Duration::ONE_SECOND);

		AnimTime systemTime;

		LARGE_INTEGER counterStruct;
		LARGE_INTEGER frequencyStruct;

		if (0 != QueryPerformanceCounter(&counterStruct) && 0 != QueryPerformanceFrequency(&frequencyStruct))
		{
			const LONGLONG counter = counterStruct.QuadPart;
			const LONGLONG frequency = frequencyStruct.QuadPart;

			if (counter >= 0 && 0 != frequency)
			{
				const int numSeconds = static_cast<int>(counter / frequency);
				const float remainder = static_cast<float>(counter % frequency);

				systemTime = numSeconds * oneSecond;
				systemTime += (remainder / (float)frequency) * oneSecond;
			}
		}

		return(systemTime);
	}
}

// ---  End of File ---
