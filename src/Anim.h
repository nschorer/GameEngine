//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef ANIM_H
#define ANIM_H

#include "AnimTimer.h"
#include "GameObjectAnim.h"
#include "Clip.h"
#include "Skeleton.h"
#include "ChunkHeader.h"

namespace Azul
{ 
	#define BONE_WIDTH 10.0f

	class Anim
	{
	public:
		Anim() = delete;
		Anim(const Anim &) = delete;
		Anim &operator = (const Anim &) = delete;
		~Anim();

		Anim(const char* const pModelFileName, CustomModel* pModel = nullptr);
		Anim(Clip* pClip);

		void Animate(AnimTime tCurr);
		void SetBoneWorlds(AnimTime tCurr);
		AnimTime FindMaxTime();

		void SetClip(Clip* pClip);
		void TransitionToClip(Clip* pClip);
		const Skeleton* GetSkeleton();


	private:

		void privParseAnimFile(const char* const pAnimFileName);
		void privParseSkel(SkeletonHeader* skelHdr, unsigned char*& p);
		void privParseHTable(HTableHeader* hHdr, unsigned char*& p);
		void privParseClip(ClipHeader* clipHdr, unsigned char*& p);


	private:
		Clip           *pClip;
		Skeleton       *poSkeleton;
		SSBO		   *poSSBO_BoneResult;
		SSBO		   *poSSBO_HierarchyTable;
		SSBO		   *poSSBO_Worlds;
		SSBO		   *poSSBO_Helper;
		Matrix* pComputeResult;
		Clip		   *pTransitionClip;

		int   numBones;
		float boneWidth;
		float transition_t;
		bool inTransition;
	};


}

#endif

// --- End of File ---
