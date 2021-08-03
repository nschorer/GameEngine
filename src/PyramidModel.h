//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef PYRAMID_MODEL_H
#define PYRAMID_MODEL_H

#include "Model.h"

namespace Azul
{
	class PyramidModel : public Model
	{
	public:
		PyramidModel(const char* const pModelFileName);

		PyramidModel() = delete;
		PyramidModel(const PyramidModel&) = delete;
		PyramidModel& operator = (PyramidModel&) = delete;
		virtual ~PyramidModel();

		void SaveToFile();

	private:
		virtual void privCreateVAO(const char* const pModelFileName) override;
		void privCreateVAO();

		// Data

	};
}

#endif

// --- End of File ---
