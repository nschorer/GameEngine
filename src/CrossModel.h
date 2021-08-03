//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef CROSS_MODEL_H
#define CROSS_MODEL_H

#include "Model.h"

namespace Azul
{
	class CrossModel : public Model
	{
	public:
		CrossModel(const char* const pModelFileName);

		CrossModel() = delete;
		CrossModel(const CrossModel&) = delete;
		CrossModel& operator = (CrossModel&) = delete;
		virtual ~CrossModel();

		void SaveToFile();

	private:
		virtual void privCreateVAO(const char* const pModelFileName) override;

		// Data

	};
}

#endif

// --- End of File ---
