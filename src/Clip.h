//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef CLIP_H
#define CLIP_H

#include "Bone.h"
#include "AnimTime.h"
#include "ClipLink.h"
#include "SSBO.h"

namespace Azul
{
	class Model;
	class Skeleton;

	class Clip : public ClipLink
	{
	public:

		enum class Name
		{
			DEFAULT,

			TEDDY_WALK = 5,
			TEDDY_IDLE,

			HUMAN_RUN = 20,
			HUMAN_WALK,
			HUMAN_PUNCH,
			HUMAN_DIE,
			HUMAN_RUN_NOT_COMPRESSED,
			HUMAN_WALK_NOT_COMPRESSED
		};



		struct FrameBucket
		{
			FrameBucket()
			{
				pSSBO_Bones = new SSBO();
			}
			~FrameBucket()
			{
				delete[] poBone;
				delete pSSBO_Bones;
			}

			FrameBucket *nextBucket;
			FrameBucket *prevBucket;
			AnimTime     KeyTime;
			Bone        *poBone;
			SSBO* pSSBO_Bones;
		};

		

	public:
		Clip() = delete;
		Clip(const Clip &) = delete;
		Clip &operator = (const Clip &) = delete;
		~Clip();

		Clip(unsigned char* p, Name _name, int _numBones, int _numFrames, AnimTime _TotalTime, unsigned int _clipSize);
		Clip(unsigned char* p, Name _name, int _numBones, int _numFrames, int* hTable, int numInts, AnimTime _TotalTime, unsigned int _clipSize);

		Name GetName();
		AnimTime GetTotalTime();
		void AnimateBones(AnimTime tCurr, SSBO*& pSSBO_Result);

	private:
		void privSetAnimationDataCompute(unsigned char* p);
		void privSetAnimationDataCompute(unsigned char* p, int* hTable, int numInts);

	public:
		int          numBones;
		int          numFrames;

		AnimTime         TotalTime;
		FrameBucket *poHead;
		Name name;
		SSBO* poSSBO_Hierarchy;
		SSBO* poSSBO_Result;

	public:
		Skeleton* poSkeleton; // each clip owns its own skeleton... for now
		Model* pModel;

		unsigned int clipSize; // for demo
	};
}

#endif

//--- End of File ---
