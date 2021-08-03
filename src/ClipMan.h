#ifndef CLIP_MAN_H
#define CLIP_MAN_H

#include "Clip.h"
#include "ChunkHeader.h"
#include "Model.h"

namespace Azul
{

	// Singleton
	class ClipMan
	{
	public:
		static void Add(Clip* pClip);
		static Clip* Find(Clip::Name name);

		static void Destroy();
		static void Create();


		~ClipMan() = default;
		ClipMan(const ClipMan&) = delete;
		ClipMan& operator=(const ClipMan&) = delete;

		static void ImportClips(const char* const filename);
		static void ImportClips2(const char* const filename, Model* pModel = nullptr);

	private:
		ClipMan();
		static ClipMan* privGetInstance();

		static void privParseClip(ClipHeader* clipHdr, unsigned char*& p, int numBones);

		static void privParseSkeleton(SkeletonHeader* skelHdr, unsigned char*& p, int& numBones, SkeletonData*& pSkeletonData);
		static void privParseHTable(HTableHeader* hHdr, unsigned char*& p, int numBones, int*& hTable, int& numInts);
		static void privParseClip(ClipHeader* clipHdr, unsigned char*& p, SkeletonData* pSkeletonData, int numBones, int* hTable, int numInts, Model* pModel);

		void privAddToFront(ClipLink* node, ClipLink*& head);
		void privRemove(ClipLink* pNode, ClipLink*& poHead);

	private:
		ClipLink* active;
	};

}

#endif