//----------------------------------------------------------------------------
// Copyright 2021, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef TEDDY_MODEL_H
#define TEDDY_MODEL_H

#include "Model.h"

namespace Azul
{
	class TeddyModel : public Model
	{
	public:
		TeddyModel(const char *const modelFileName);
		virtual ~TeddyModel();

		// tells the compiler do not create or allow it to be used, c++11
		TeddyModel(const TeddyModel &) = delete;
		TeddyModel &operator=(const TeddyModel &other) = delete;

	public:
		void privCreateVAO(const char *const modelFileName) override;

		GLuint vbo_boneweight;   // Bone_weight
		GLuint vbo_boneindex;   // Bone_index

		unsigned int NumBones;
		Matrix *poInversePose;
		Matrix *poBone;
	};
}

#endif

// --- End of File ---
