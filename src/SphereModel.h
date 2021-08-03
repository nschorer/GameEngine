//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef SPHERE_MODEL_H
#define SPHERE_MODEL_H

#include "Model.h"

namespace Azul
{
	class SphereModel : public Model
	{
	public:
		SphereModel(const char* const pModelFileName);

		SphereModel() = delete;
		SphereModel(const SphereModel&) = delete;
		SphereModel& operator = (SphereModel&) = delete;
		virtual ~SphereModel();

		void SaveToFile();

	private:
		virtual void privCreateVAO(const char* const pModelFileName) override;

		// Data

	};
}

#endif

// --- End of File ---
