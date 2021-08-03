//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef MIXER_H
#define MIXER_H

#include "AnimTimer.h"
#include "Bone.h"
#include "SSBO.h"
#include "Clip.h"

namespace Azul
{
	class Mixer
	{
	public:

		~Mixer();
		Mixer(const Mixer& rhs) = delete;
		Mixer& operator=(const Mixer& rhs) = delete;

		static void Create();
		static void Destroy();

		static void Blend(	Bone *pResult,
							const Bone *pA,
							const Bone *pB,
							const float tS,
							const int numBones);

		static void BlendCompute(Bone* pResult,
			const Bone* pA,
			const Bone* pB,
			const float tS,
			const int numBones);

		static void BlendCompute(SSBO* pKey_A,
			SSBO* PKey_B,
			SSBO* pResult_GPU,
			const float tS,
			const int numBones);


	private:
		Mixer();
		static Mixer* privGetInstance();

	private:
		SSBO* pSSBO_in1;
		SSBO* pSSBO_in2;
		SSBO* pSSBO_out;
	};


}

#endif

// --- End of File ---
