//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef SKELETON_H
#define SKELETON_H

#include "GameObjectAnim.h"
#include "Bone.h"
#include "Model.h"
#include "SSBO.h"

namespace Azul
{

	class Skeleton
	{
	public:
		struct Data
		{
			char name[64];
			int  index;
			int  parentIndex;
		};

	public:
		Skeleton(SkeletonData* pBoneH, int numBones);


		Skeleton() = delete;
		Skeleton(const Skeleton &) = delete;
		Skeleton &operator = (const Skeleton &) = delete;
		~Skeleton() = default;

		GameObjectAnim *GetFirstBone();
		void SetModel(Model* pModel);

		Model* pModel; // make this private

	private:
		GameObjectAnim *privFindBoneByIndex(int index);

		void privSetAnimationHierarchy(SkeletonData* pBoneH);

	public:
		GameObjectAnim *pFirstBone;
		int             numBones;
	};
}

#endif

// --- End of File ---
