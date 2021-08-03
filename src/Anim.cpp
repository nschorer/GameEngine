//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Anim.h"
#include "Constants.h"
#include "MathEngine.h"
#include "MathApp.h"
#include "Mixer.h"
#include "PCSTreeForwardIterator.h"
#include "GameObjectMan.h"
#include "ClipMan.h"
#include "File.h"
#include "PackageHeader.h"
#include "Bone.h"
#include "ShaderMan.h"

//#define DEBUG_POSE

namespace Azul
{

	Anim::~Anim()
	{
		delete poSSBO_BoneResult;
		delete poSSBO_Helper;
		delete poSSBO_HierarchyTable;
		delete poSSBO_Worlds;
		//delete poSkeleton;
		delete[] pComputeResult;
	}

	Anim::Anim(const char* const pAnimFileName, CustomModel* pModel)
		: pClip(nullptr),
		poSkeleton(nullptr),
		numBones(0),
		boneWidth(BONE_WIDTH),
		transition_t(0),
		inTransition(false)
	{
		// init before parsing file
		poSSBO_HierarchyTable = new SSBO();

		privParseAnimFile(pAnimFileName);

		assert(numBones > 0);
		assert(poSkeleton);
		assert(pClip);

		unsigned int num = (unsigned int)numBones;

		poSSBO_BoneResult = new SSBO();
		poSSBO_Helper = new SSBO();     // used in transitions
		Bone* pBone = new Bone[num];
		poSSBO_BoneResult->Set(num, sizeof(Bone), pBone);
		poSSBO_Helper->Set(num, sizeof(Bone), pBone);
		delete[] pBone; // it's on the GPU now

		poSSBO_Worlds = new SSBO();
		pComputeResult = new Matrix[(unsigned int)num];
		poSSBO_Worlds->Set(num, sizeof(Matrix), pComputeResult);

		poSkeleton->SetModel(pModel);
	}

	Anim::Anim(Clip* _pClip)
		: pClip(_pClip),
		poSkeleton(nullptr),
		pTransitionClip(nullptr),
		numBones(0),
		boneWidth(BONE_WIDTH),
		transition_t(0),
		inTransition(false)
	{
		// init before parsing file
		poSSBO_HierarchyTable = new SSBO();

		assert(pClip);

		// for now, we're just poaching everything from Clip
		numBones = pClip->numBones;
		poSkeleton = pClip->poSkeleton;
		poSkeleton->SetModel(pClip->pModel);

		unsigned int num = (unsigned int)numBones;

		poSSBO_BoneResult = new SSBO();
		poSSBO_Helper = new SSBO();     // used in transitions
		Bone* pBone = new Bone[num];
		poSSBO_BoneResult->Set(num, sizeof(Bone), pBone);
		poSSBO_Helper->Set(num, sizeof(Bone), pBone);
		delete[] pBone; // it's on the GPU now

		poSSBO_Worlds = new SSBO();
		pComputeResult = new Matrix[(unsigned int)num];
		poSSBO_Worlds->Set(num, sizeof(Matrix), pComputeResult);

		


		//poSkeleton->SetModel(pModel);
	}

	AnimTime Anim::FindMaxTime()
	{
		assert(pClip);
		return this->pClip->GetTotalTime();
	}

	void Anim::SetClip(Clip* _pClip)
	{
		assert(_pClip->numBones == this->numBones);
		this->pClip = _pClip;
	}

	void Anim::TransitionToClip(Clip* _pClip)
	{
		assert(_pClip->numBones == this->numBones);
		pTransitionClip = _pClip;
		inTransition = true;
		transition_t = 0;
	}

	const Skeleton* Anim::GetSkeleton()
	{
		return this->poSkeleton;
	}

	void Anim::Animate(AnimTime tCurr)
	{
		SSBO* pSSBO_ClipResultA;
		this->pClip->AnimateBones(tCurr, pSSBO_ClipResultA);

		if (!inTransition)
		{
			// Pour the contents of the clip into the Anim object
			pSSBO_ClipResultA->CopyToSSBO(poSSBO_BoneResult);
		}
		else
		{
			SSBO* pSSBO_ClipResultB;
			this->pTransitionClip->AnimateBones(tCurr, pSSBO_ClipResultB);


			float t_delta = 0.015f;
			float b = 1.0f;
			float a = 0.0f;
			transition_t += t_delta;

			// fully transitioned
			if (transition_t > 1.0f)
			{
				transition_t = 1.0f;
				inTransition = false;
				pClip = pTransitionClip;
				pTransitionClip = nullptr;
			}

			float tS = a + transition_t * (b - a);

			Mixer::BlendCompute(pSSBO_ClipResultA, pSSBO_ClipResultB, this->poSSBO_BoneResult, tS, this->numBones);

		}

		
	}

	void Anim::SetBoneWorlds(AnimTime tCurr)
	{
		SSBO*& bone = poSSBO_BoneResult;
		//SSBO*& hierarchy = poSSBO_HierarchyTable; // set this (and depth) on import and never change?
		SSBO*& hierarchy = pClip->poSSBO_Hierarchy; // set this (and depth) on import and never change?
		SSBO*& out = poSSBO_Worlds;

		bone->Bind(0);
		hierarchy->Bind(1); // still need to rebind though
		out->Bind(2);

		unsigned int num = (unsigned int)numBones;

		ShaderObject* pComputeShader = ShaderMan::Find(ShaderObject::Name::COMPUTE_LOCAL_TO_WORLD);
		assert(pComputeShader);
		pComputeShader->SetActive();

		// hard code to groups of 20 for now
		unsigned int numBatches = num / 20 + (num % 20 != 0);
		pComputeShader->Dispatch(numBatches, 1, 1);

		// Block Waiting for stages to complete
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);



		// Retrieve the result from the compute shader and shove it in the skeleton
		memcpy(pComputeResult, out->Map(SSBO::Access::READ_ONLY), sizeof(Matrix) * num);
		// needed?
		out->Unmap();



		PCSTreeForwardIterator pIter(poSkeleton->pFirstBone);
		PCSNode* pNode = pIter.First();
		GameObjectAnim* pGameObj = nullptr;


		// walks the anim node does the pose for everything that
		while (pNode != nullptr)
		{
			assert(pNode);
			// Update the game object
			pGameObj = (GameObjectAnim*)pNode;
			int idx = pGameObj->indexBoneArray;
			Matrix& m = pComputeResult[idx];
			pGameObj->GetWorld()->set(m);

			pNode = pIter.Next();
		}
	}

	void Anim::privParseAnimFile(const char* const pAnimFileName)
	{
		// File stuff
		File::Handle fh;
		File::Error  ferror;

		// READ the data from the file ONLY

		ferror = File::Open(fh, pAnimFileName, File::Mode::READ);
		assert(ferror == File::Error::SUCCESS);

		// Read the Package Header to find out how much is in the file
		PackageHeader pHdr;

		ferror = File::Read(fh, &pHdr, sizeof(PackageHeader));
		assert(ferror == File::Error::SUCCESS);

		unsigned int numChunks = pHdr.numChunks;
		unsigned int packageSize = pHdr.packageSize;
		char animName[PackageHeader::PACKAGE_NAME_SIZE];
		strcpy_s(animName, PackageHeader::PACKAGE_NAME_SIZE, pHdr.packageName);
		char animVersion[PackageHeader::PACKAGE_VERSION_SIZE];
		strcpy_s(animVersion, PackageHeader::PACKAGE_VERSION_SIZE, pHdr.versionString);

		unsigned char* buffer = new unsigned char[packageSize];

		ferror = File::Read(fh, buffer, packageSize);
		unsigned char* p = (unsigned char*)buffer;
		unsigned char* bufferBottom = p + packageSize;


		// Read all the chunks -- each chunk is an animation clip
		for (unsigned int i = numChunks; i > 0; i--) {
			// Figure out what kind of chunk we're dealing with

			ChunkHeader* cHdr = (ChunkHeader*)p;

			switch (cHdr->type) {
				

				case ChunkType::SKEL_TYPE:
				{
					SkeletonHeader* skelHdr = (SkeletonHeader*)cHdr;
					privParseSkel(skelHdr, p);
					break;
				}

				case ChunkType::H_TABLE_TYPE:
				{
					HTableHeader* hHdr = (HTableHeader*)cHdr;
					privParseHTable(hHdr, p);
					break;
				}

				case ChunkType::CLIP_TYPE:
				{
					ClipHeader* clipHdr = (ClipHeader*)cHdr;
					privParseClip(clipHdr, p);
					break;
				}
				case ChunkType::NORMS_TYPE:
				case ChunkType::SPHERE_TYPE:
				case ChunkType::TEXTURE_TYPE:
				case ChunkType::TRIS_TYPE:
				case ChunkType::UV_TYPE:
				case ChunkType::VERTS_TYPE:
				case ChunkType::BONE_WEIGHT_TYPE:
				case ChunkType::BONE_IDX_TYPE:
				case ChunkType::INV_MAT_TYPE:

				default:
				{
					assert(false); // Not implemented 
					break;
				}
			}
		}

		assert(p == bufferBottom);
		delete[] buffer;
	}


	void Anim::privParseSkel(SkeletonHeader* skelHdr, unsigned char*& p)
	{
		assert(skelHdr->numBones > 0);

		this->numBones = skelHdr->numBones;

		p += sizeof(SkeletonHeader);

		this->poSkeleton = new Skeleton((SkeletonData*)p, this->numBones);

		p += skelHdr->chunkSize;
	}

	void Anim::privParseHTable(HTableHeader* hHdr, unsigned char*& p)
	{
		assert(hHdr->numBones == this->numBones);
		//this->numBones = hHdr->numBones;
		int depth = hHdr->depth;

		p += sizeof(HTableHeader);

		unsigned int num = (unsigned int)(1 + 1 + depth * this->numBones);

		int* tmp = new int[num * sizeof(int)];

		memcpy(tmp, &depth, sizeof(int));
		memcpy(tmp + 1, &this->numBones, sizeof(int));
		memcpy(tmp + 2, p, sizeof(int) * depth * this->numBones);

		poSSBO_HierarchyTable->Set(num, sizeof(int), tmp);

		delete[] tmp;

		p += hHdr->chunkSize;
	}

	// Read the clip, advance the pointer
	void Anim::privParseClip(ClipHeader* clipHdr, unsigned char*& p)
	{
		assert(this->numBones > 0); // this means skeleton MUST be processed before clips
		assert(clipHdr->numFrames > 0);
		//assert(animHdr->TotalTime > 0);

		int numFrames = clipHdr->numFrames;
		float totalTime = clipHdr->totalTime;
		AnimTime TotalTime = Azul::AnimTime(Azul::AnimTime::Duration::ONE_SECOND) * (totalTime / 30.0f);

		p += sizeof(ClipHeader); // increment p

		// Get this from the AnimMan
		Clip* tmp = new Clip(p, (Clip::Name)clipHdr->name, this->numBones, numFrames, TotalTime, clipHdr->chunkSize);
		if (this->pClip == nullptr)
		{
			//privAddClip(tmp);
			this->pClip = tmp;
		}

		ClipMan::Add(tmp);

		p += clipHdr->chunkSize;

	}

	//void Anim::privAddClip(Clip* _pClip)
	//{
	//	// Circular linked list

	//	assert(_pClip != nullptr);

	//	if (this->pClip == nullptr)
	//	{
	//		this->pClip = _pClip;
	//		_pClip->next = _pClip;
	//		_pClip->prev = _pClip;
	//	}

	//	else
	//	{
	//		Clip* pHead = this->pClip;
	//		Clip* pTail = (Clip*)this->pClip->prev;

	//		pTail->next = _pClip;
	//		pHead->prev = _pClip;
	//		_pClip->next = pHead;
	//		_pClip->prev = pTail;
	//	}
	//}
}

//--- End of File ----
