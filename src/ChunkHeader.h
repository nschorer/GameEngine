//-----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 


#include <stdint.h>
#include "Texture.h"
#include "Clip.h"
#include "AnimTime.h"

#ifndef CHUNK_HEADER_H
#define CHUNK_HEADER_H

namespace Azul {

	enum class ChunkType : uint32_t
	{
		VERTS_TYPE = 0xA0000000u,
		NORMS_TYPE,
		TEXTURE_TYPE,
		UV_TYPE,
		TRIS_TYPE,
		SPHERE_TYPE,
		SKEL_TYPE,
		CLIP_TYPE,
		BONE_WEIGHT_TYPE,
		BONE_IDX_TYPE,
		INV_MAT_TYPE,
		H_TABLE_TYPE
	};

	struct ChunkHeader
	{
		ChunkHeader() = default;
		ChunkHeader(ChunkType t) : type(t), chunkName("DEFAULT"), chunkSize(0), hashNum(0) {};

		static const unsigned int CHUNK_NAME_SIZE = 20;

		// data:
		ChunkType      type;
		char           chunkName[CHUNK_NAME_SIZE];
		unsigned int   chunkSize;
		unsigned int   hashNum;
	};

	struct VertsHeader : ChunkHeader
	{
		VertsHeader()
			: ChunkHeader(ChunkType::VERTS_TYPE), numVerts(0)
		{

		}

		// vertex buffer
		unsigned int		numVerts;
		//unsigned int	vertBufferOffset;


	};

	struct TrisHeader : ChunkHeader
	{
		TrisHeader()
			: ChunkHeader(ChunkType::TRIS_TYPE), numTriList(0)
		{

		}
		// trilist index 
		unsigned int		numTriList;
		//unsigned int	triListBufferOffset;
	};

	struct TextureHeader : ChunkHeader
	{
		TextureHeader()
			: ChunkHeader(ChunkType::TEXTURE_TYPE), fName(""), eName(Texture::Name::DEFAULT)
		{

		}

		// nothing? We're just gonna plop the whole tga in there
		static const unsigned int TEXTURE_NAME_SIZE = 32;
		char fName[TEXTURE_NAME_SIZE];
		Texture::Name eName;
	};

	struct SphereHeader : ChunkHeader
	{
		SphereHeader()
			: ChunkHeader(ChunkType::SPHERE_TYPE), x(0.0f), y(0.0f), z(0.0f), r(0.0f)
		{
		}

		float x, y, z, r;
	};

	struct SkeletonHeader : ChunkHeader
	{
		SkeletonHeader()
			:ChunkHeader(ChunkType::SKEL_TYPE), numBones(0)
		{

		}

		int numBones;
	};

	struct ClipHeader : ChunkHeader
	{
		ClipHeader()
			: ChunkHeader(ChunkType::CLIP_TYPE), numFrames(0), totalTime(0), name(Clip::Name::DEFAULT)
		{

		}

		int          numFrames;
		float        totalTime;
		Clip::Name	 name;
	};

	struct BoneWeightHeader : ChunkHeader
	{
		BoneWeightHeader() : ChunkHeader(ChunkType::BONE_WEIGHT_TYPE), numVerts(0)
		{}

		int numVerts;
	};

	struct BoneIdxHeader : ChunkHeader
	{
		BoneIdxHeader() : ChunkHeader(ChunkType::BONE_IDX_TYPE), numVerts(0)
		{}

		int numVerts;
	};

	struct InvMatHeader : ChunkHeader
	{
		InvMatHeader() : ChunkHeader(ChunkType::INV_MAT_TYPE), numBones(0)
		{
		}

		int numBones;
	};

	struct HTableHeader : ChunkHeader
	{
		HTableHeader()
			: ChunkHeader(ChunkType::H_TABLE_TYPE), numBones(0), depth(0)
		{
		}

		int numBones;
		int depth;
	};

	// ***************************************************
	// Helper structs
	// ***************************************************

	struct Vert_xyzuvn
	{
		float x;
		float y;
		float z;
		float u;
		float v;
		float nx;
		float ny;
		float nz;
	};

	struct Tri_index
	{
		unsigned int v0;
		unsigned int v1;
		unsigned int v2;
	};

	struct Bone_weight
	{
		float a;
		float b;
		float c;
		float d;
	};

	struct Bone_index
	{
		unsigned int ia;
		unsigned int ib;
		unsigned int ic;
		unsigned int id;
	};

	struct Mat
	{
		float m0;
		float m1;
		float m2;
		float m3;
		float m4;
		float m5;
		float m6;
		float m7;
		float m8;
		float m9;
		float m10;
		float m11;
		float m12;
		float m13;
		float m14;
		float m15;
	};

	struct SkeletonData
	{
		char name[64];
		int  index;
		int  parentIndex;
	};

	struct FrameBucketHdr // support header for AnimHeader
	{
		float keyTime;
	};
}

#endif