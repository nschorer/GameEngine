//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include <string.h>
#include "ModelFileHdr.h"

namespace Azul
{
	ModelFileHdr::ModelFileHdr()
	{
		memset(this, 0x0, sizeof(ModelFileHdr));
	}

}
// --- End of File ---
