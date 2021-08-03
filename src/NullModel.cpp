//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "NullModel.h"

namespace Azul
{
	NullModel::NullModel(const char* const modelFileName)
		: Model()
	{
		AZUL_UNUSED_VAR(modelFileName);
	}

	NullModel::~NullModel()
	{
		// remove anything dynamic here
	}

	void NullModel::privCreateVAO(const char* const modelFileName)
	{
		AZUL_UNUSED_VAR(modelFileName);
	}
}

// --- End of File ---
