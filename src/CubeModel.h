//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef CUBE_MODEL_H
#define CUBE_MODEL_H

#include "Model.h"

namespace Azul
{
	class CubeModel : public Model
	{
	public:
		CubeModel(const char* const pModelFileName);

		CubeModel() = delete;
		CubeModel(const CubeModel&) = delete;
		CubeModel& operator = (CubeModel&) = delete;
		virtual ~CubeModel();

	private:
		virtual void privCreateVAO(const char* const pModelFileName) override;

		// Data

	};
}

#endif

// --- End of File ---
