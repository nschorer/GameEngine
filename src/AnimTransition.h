//----------------------------------------------------------------------------
// Copyright 2021, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef ANIM_TRANSITION_H
#define ANIM_TRANSITION_H

#include "TimerController.h"
#include "GameObjectAnim.h"
#include "Anim.h"

namespace Azul
{
	class AnimTransition
	{
	public:
		AnimTransition(Clip *pClip, Clip *pClipB);
		AnimTransition(const AnimTransition&) = delete;
		AnimTransition& operator = (const AnimTransition&) = delete;
		~AnimTransition();

		void Update();

		void StartTransition();
		void privTransitionTime();

	public:
		SSBO* pSSBO_A;
		SSBO* pSSBO_B;
		SSBO* poSSBO_Result;

		SSBO* pSSBO_Hierarchy;
		SSBO* poBoneWorld_SBO;

		int numBones;
		float tS;
		bool StartEnable;
	};
}

#endif

// --- End of File ---

