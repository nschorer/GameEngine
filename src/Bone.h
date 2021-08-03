//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef BONE_H
#define BONE_H

#include "MathEngine.h"

namespace Azul
{
	struct Bone : public Align16
	{
		Vect  T;
		Quat  Q;
		Vect  S;
	};
}

#endif

// --- End of File ---
