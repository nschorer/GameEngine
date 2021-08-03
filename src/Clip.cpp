
//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Clip.h"
#include "Time.h"
#include "Bone.h"
#include "Mixer.h"
#include "ChunkHeader.h"
#include "Model.h"
#include "Skeleton.h"

//#define DEBUG_CLIP

namespace Azul
{
	Clip::Clip(unsigned char* p, Name _name, int _numBones, int _numFrames, AnimTime _TotalTime, unsigned int _clipSize)
		: numBones(_numBones),
		numFrames(_numFrames),
		TotalTime(_TotalTime),
		poHead(nullptr),
		name(_name),
		poSkeleton(nullptr),
		clipSize(_clipSize)
	{
		poSSBO_Result = new SSBO();
		poSSBO_Hierarchy = new SSBO();
		this->privSetAnimationDataCompute(p);
	}

	Clip::Clip(unsigned char* p, Name _name, int _numBones, int _numFrames, int* hTable, int numInts, AnimTime _TotalTime, unsigned int _clipSize)
		: numBones(_numBones),
		numFrames(_numFrames),
		TotalTime(_TotalTime),
		poHead(nullptr),
		name(_name),
		poSkeleton(nullptr),
		clipSize(_clipSize)
	{
		poSSBO_Result = new SSBO();
		poSSBO_Hierarchy = new SSBO();
		this->privSetAnimationDataCompute(p, hTable, numInts);
		//assert(poSkeleton);

		Bone* pBone = new Bone[(unsigned int)numBones];
		poSSBO_Result->Set((unsigned int)numBones, sizeof(Bone), pBone);
		delete[] pBone; // it's on the GPU now
	}

	Clip::~Clip()
	{
		delete[] poHead;
		delete poSSBO_Result;
		delete poSSBO_Hierarchy;
		if (poSkeleton != nullptr)
		{
			delete poSkeleton;
		}
	}

	void Clip::AnimateBones(AnimTime tCurr, SSBO*& pResult)
	{
		// First one 
		FrameBucket* pTmp = this->poHead;

		// Find which key frames
		while (tCurr >= pTmp->KeyTime && pTmp->nextBucket != nullptr)
		{
			pTmp = pTmp->nextBucket;
		}

		// pTmp is the "B" key frame
		// pTmp->prev is the "A" key frame
		FrameBucket* pA = pTmp->prevBucket;
		FrameBucket* pB = pTmp;

		// find the "S" of the time
		float tS = (tCurr - pA->KeyTime) / (pB->KeyTime - pA->KeyTime);

		Mixer::BlendCompute(pA->pSSBO_Bones, pB->pSSBO_Bones, poSSBO_Result, tS, numBones);
		pResult = poSSBO_Result;
	}

	Clip::Name Clip::GetName()
	{
		return name;
	}

	AnimTime Clip::GetTotalTime()
	{
		return this->TotalTime;
	}

	void Clip::privSetAnimationDataCompute(unsigned char* p)
	{
		FrameBucket* pLast = nullptr;
		poHead = new FrameBucket[(unsigned int)this->numFrames];

		for (int i = 0; i < this->numFrames; i++) {

			FrameBucket* pTmp = &poHead[i];
			pTmp->prevBucket = pLast;
			pTmp->nextBucket = nullptr;
			//pTmp->KeyTime = i * AnimTime(AnimTime::Duration::NTSC_30_FRAME);
			pTmp->poBone = new Bone[(unsigned int)this->numBones];

			FrameBucketHdr* pBktHdr = (FrameBucketHdr*)p;
			pTmp->KeyTime = pBktHdr->keyTime * AnimTime(AnimTime::Duration::NTSC_30_FRAME);
			p += sizeof(FrameBucketHdr);

			// store bones on GPU
			pTmp->pSSBO_Bones->Set((unsigned int)numBones, sizeof(Bone), p);
			p += sizeof(Bone) * numBones;

			if (pLast != nullptr) {
				pLast->nextBucket = pTmp;
			}

			pLast = pTmp;
		}
		
	}

	void Clip::privSetAnimationDataCompute(unsigned char* p, int* hTable, int numInts)
	{
		poSSBO_Hierarchy->Set((unsigned int)numInts, sizeof(int), hTable);

		FrameBucket* pLast = nullptr;
		poHead = new FrameBucket[(unsigned int)this->numFrames];

		for (int i = 0; i < this->numFrames; i++) {

			FrameBucket* pTmp = &poHead[i];
			pTmp->prevBucket = pLast;
			pTmp->nextBucket = nullptr;
			//pTmp->KeyTime = i * AnimTime(AnimTime::Duration::NTSC_30_FRAME);
			pTmp->poBone = new Bone[(unsigned int)this->numBones];

			FrameBucketHdr* pBktHdr = (FrameBucketHdr*)p;
			pTmp->KeyTime = pBktHdr->keyTime * AnimTime(AnimTime::Duration::NTSC_30_FRAME);
			p += sizeof(FrameBucketHdr);

			// store bones on GPU
			pTmp->pSSBO_Bones->Set((unsigned int)numBones, sizeof(Bone), p);
			p += sizeof(Bone) * numBones;

			if (pLast != nullptr) {
				pLast->nextBucket = pTmp;
			}

			pLast = pTmp;
		}
	}
	
}

// --- End of File ---
