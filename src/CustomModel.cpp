//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "CustomModel.h"
#include "PackageHeader.h"
#include "File.h"

namespace Azul {
	CustomModel::CustomModel(const char* const pModelFileName)
	{
		assert(pModelFileName);
		this->privCreateVAO(pModelFileName);
	}	

	CustomModel::~CustomModel()
	{
		// Clean up anim component
		if (numBones > 0)
		{
			delete[] this->poInversePose;
			delete[] this->poBone;
		}
	}



	void CustomModel::privCreateVAO(const char* const pModelFileName)
	{
		// Create a VAO
		glGenVertexArrays(1, &this->vao);
		assert(this->vao != 0);
		glBindVertexArray(this->vao);

		// Create VBOs
		glGenBuffers(1, &this->vbo_verts);
		assert(this->vbo_verts != 0);

		glGenBuffers(1, &this->vbo_trilist);
		assert(this->vbo_trilist != 0);

		glGenBuffers(1, &this->vbo_boneweight);
		assert(this->vbo_boneweight != 0);

		glGenBuffers(1, &this->vbo_boneindex);
		assert(this->vbo_boneindex != 0);


		// File stuff
		File::Handle fh;
		File::Error  ferror;

		// READ the data from the file ONLY

		ferror = File::Open(fh, pModelFileName, File::Mode::READ);
		assert(ferror == File::Error::SUCCESS);

		// Read the Package Header to find out how much is in the file
		PackageHeader pHdr;

		ferror = File::Read(fh, &pHdr, sizeof(PackageHeader));
		assert(ferror == File::Error::SUCCESS);

		unsigned int numChunks = pHdr.numChunks;
		unsigned int packageSize = pHdr.packageSize;
		char modelName[PackageHeader::PACKAGE_NAME_SIZE];
		strcpy_s(modelName, PackageHeader::PACKAGE_NAME_SIZE, pHdr.packageName);
		char modelVersion[PackageHeader::PACKAGE_VERSION_SIZE];
		strcpy_s(modelVersion, PackageHeader::PACKAGE_VERSION_SIZE, pHdr.versionString);

		unsigned char* buffer = new unsigned char[packageSize];

		ferror = File::Read(fh, buffer, packageSize);
		unsigned char* p = (unsigned char*)buffer;

		// Read all the chunks
		for (unsigned int i = numChunks; i > 0; i--) {
			// Figure out what kind of chunk we're dealing with

			ChunkHeader* cHdr = (ChunkHeader*)p;

			switch (cHdr->type) {
			case ChunkType::VERTS_TYPE:
			{
				VertsHeader* vHdr = (VertsHeader*)cHdr;
				privParseVerts(vHdr, p);
				break;
			}
			case ChunkType::TRIS_TYPE:
			{
				TrisHeader* triHdr = (TrisHeader*)cHdr;
				privParseTris(triHdr, p);
				break;
			}
			case ChunkType::TEXTURE_TYPE:
			{
				TextureHeader* textHdr = (TextureHeader*)cHdr;
				privParseText(textHdr, p);
				break;
			}

			case ChunkType::SPHERE_TYPE:
			{
				SphereHeader* sHdr = (SphereHeader*)cHdr;
				privParseSphere(sHdr, p);
				break;
			}

			case ChunkType::BONE_WEIGHT_TYPE:
			{
				BoneWeightHeader* bHdr = (BoneWeightHeader*)cHdr;
				privParseBoneWeights(bHdr, p);
				break;
			}
			case ChunkType::BONE_IDX_TYPE:
			{
				BoneIdxHeader* bHdr = (BoneIdxHeader*)cHdr;
				privParseBoneIdxs(bHdr, p);
				break;
			}
			case ChunkType::INV_MAT_TYPE:
			{
				InvMatHeader* imHdr = (InvMatHeader*)cHdr;
				privParseInvMats(imHdr, p);
				break;
			}

			case ChunkType::CLIP_TYPE:
			case ChunkType::SKEL_TYPE:
			case ChunkType::NORMS_TYPE:
			case ChunkType::UV_TYPE:
			case ChunkType::H_TABLE_TYPE:

			default:
				assert(false); // Not implemented 

				break;
			}
		}


		delete[] buffer;
	}



	// *********************
	// Parse Chunks
	// *********************




	void CustomModel::privParseVerts(VertsHeader* vHdr, unsigned char*& p)
	{
		assert(vHdr->numVerts > 0);
		this->numVerts = (int)vHdr->numVerts;
		Vert_xyzuvn* vBuffer = new Vert_xyzuvn[(unsigned int)this->numVerts];

		p += sizeof(VertsHeader);
		//p += sizeof(VertsHeader); // THIS IS A BUG. Fix the converter, then remove this line.

		memcpy(vBuffer, p, vHdr->chunkSize);

		p += vHdr->chunkSize; // set pointer at beginning of next chunk

		// Load the combined data: ---------------------------------------------------------

		glBindBuffer(GL_ARRAY_BUFFER, this->vbo_verts);

		// load the data to the GPU
		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Vert_xyzuvn) * this->numVerts), vBuffer, GL_STATIC_DRAW);

		// Vert data is location: 0  (used in vertex shader)
		void* offsetVert = (void*)((unsigned int)&vBuffer[0].x - (unsigned int)vBuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetVert);
		glEnableVertexAttribArray(0);

		// Texture data is location: 1  (used in vertex shader)
		void* offsetTex = (void*)((unsigned int)&vBuffer[0].u - (unsigned int)vBuffer);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetTex);
		glEnableVertexAttribArray(1);

		// normals data in location 2 (used in vertex shader
		void* offsetNorm = (void*)((unsigned int)&vBuffer[0].nx - (unsigned int)vBuffer);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetNorm);
		glEnableVertexAttribArray(2);

		delete[] vBuffer;
	}
	void CustomModel::privParseTris(TrisHeader* triHdr, unsigned char*& p)
	{
		assert(triHdr->numTriList > 0);
		this->numTris = (int)triHdr->numTriList;
		Tri_index* triBuffer = new Tri_index[(unsigned int)this->numTris];

		p += sizeof(TrisHeader);
		//p += sizeof(TrisHeader); // THIS IS A BUG. Fix the converter, then remove this line.

		memcpy(triBuffer, p, triHdr->chunkSize);

		p += triHdr->chunkSize; // set pointer at beginning of next chunk

		// Load the index data: ---------------------------------------------------------

			/* Bind our 2nd VBO as being the active buffer and storing index ) */
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vbo_trilist);

		/* Copy the index data to our buffer */
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Tri_index) * this->numTris), triBuffer, GL_STATIC_DRAW);

		delete triBuffer;
	}
	void CustomModel::privParseText(TextureHeader* textHdr, unsigned char*& p)
	{
		// Extract the tga from our .azul file

		p += sizeof(TextureHeader);
		//p += sizeof(TextureHeader); // THIS IS A BUG. Fix the converter, then remove this line.

		// Make sure not to load the texture twice
		unsigned int hash = textHdr->hashNum;
		if (!TextureMan::ChecksumAlreadyExists(hash)) {

			unsigned char* tga = new unsigned char[textHdr->chunkSize];

			memcpy(tga, p, textHdr->chunkSize);

			p += textHdr->chunkSize; // set pointer at beginning of next chunk

			// Throw the tga into it's own file in same directory
			// We're treating it just like any other tga we would import

			File::Handle fh_tga;
			File::Error ferror;

			ferror = File::Open(fh_tga, textHdr->fName, File::Mode::WRITE);
			assert(ferror == File::Error::SUCCESS);

			ferror = File::Write(fh_tga, tga, textHdr->chunkSize);
			assert(ferror == File::Error::SUCCESS);

			ferror = File::Close(fh_tga);
			delete tga;

			TextureMan::Add(textHdr->fName, textHdr->eName, hash);

		}

			// Only assign this texture if we don't have any textures yet
			if (this->textName == Texture::Name::DEFAULT) {
				this->textName = textHdr->eName;
			}
		
	}

	void CustomModel::privParseSphere(SphereHeader* sHdr, unsigned char*& p)
	{
		this->poRefSphere->cntr = Vect(sHdr->x, sHdr->y, sHdr->z);
		this->poRefSphere->rad = sHdr->r;
		this->poRefSphere->rad *= 4.2f; // this is not the solution... but I'm tired
		// I think the sphere model itself is too small. So I'm correcting it here... not sure how bad that is

		p += sizeof(SphereHeader); // chunkSize is 0
	}
	void CustomModel::privParseBoneWeights(BoneWeightHeader* bHdr, unsigned char*& p)
	{
		assert(bHdr->numVerts > 0);
		int nVerts = bHdr->numVerts;
		Bone_weight* bwBuffer = new Bone_weight[(unsigned int)nVerts];

		p += sizeof(BoneWeightHeader);

		memcpy(bwBuffer, p, bHdr->chunkSize);

		p += bHdr->chunkSize; // set pointer at beginning of next chunk

		

		glBindBuffer(GL_ARRAY_BUFFER, this->vbo_boneweight);

		// load the data to the GPU
		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Bone_weight) * nVerts), bwBuffer, GL_STATIC_DRAW);

		// BoneWeight data in location 3
		void* offsetVert = (void*)((unsigned int)&bwBuffer[0].a - (unsigned int)bwBuffer);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Bone_weight), offsetVert);
		glEnableVertexAttribArray(3);

		delete bwBuffer;
	}

	void CustomModel::privParseBoneIdxs(BoneIdxHeader* bHdr, unsigned char*& p)
	{
		assert(bHdr->numVerts > 0);
		int nVerts = bHdr->numVerts; // should we sync this with this->numVerts?
		Bone_index* biBuffer = new Bone_index[(unsigned int)nVerts]; // ints, not floats

		p += sizeof(BoneIdxHeader);

		memcpy(biBuffer, p, bHdr->chunkSize);

		p += bHdr->chunkSize; // set pointer at beginning of next chunk

		

		glBindBuffer(GL_ARRAY_BUFFER, this->vbo_boneindex);

		// load the data to the GPU
		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Bone_index) * nVerts), biBuffer, GL_STATIC_DRAW);

		// BoneIndex data in location 4
		void* offsetVert = (void*)((unsigned int)&biBuffer[0].ia - (unsigned int)biBuffer);
		glVertexAttribIPointer(4, 4, GL_UNSIGNED_INT, sizeof(Bone_index), offsetVert);
		glEnableVertexAttribArray(4);

		delete biBuffer;
	}

	void CustomModel::privParseInvMats(InvMatHeader* imHdr, unsigned char*& p)
	{
		assert(imHdr->numBones > 0);
		this->numBones = (unsigned int)imHdr->numBones;
		this->poInversePose = new Matrix[this->numBones];
		this->poBone = new Matrix[this->numBones];

		p += sizeof(InvMatHeader);

		memcpy(this->poInversePose, p, imHdr->chunkSize);

		p += imHdr->chunkSize; // set pointer at beginning of next chunk

		for (unsigned int i = 0; i < this->numBones; i++)
		{
			this->poBone[i].set(Matrix(Matrix::Special::Identity));
		}
	}
}


// --- End of File ---
