//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "CrossModel.h"
#include "ModelFileHdr.h"
#include "File.h"

namespace Azul
{
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

	void CrossModel::privCreateVAO(const char* const pModelFileName)
	{
		// File stuff
		File::Handle fh;
		File::Error  ferror;

		// create a header
		ModelFileHdr modelHdr;

		// READ the data from the file ONLY

		ferror = File::Open(fh, pModelFileName, File::Mode::READ);
		assert(ferror == File::Error::SUCCESS);

		// Read the Hdr
		ferror = File::Read(fh, &modelHdr, sizeof(ModelFileHdr));
		assert(ferror == File::Error::SUCCESS);

		// Using the hdr, allocate the space for the buffers
		this->numVerts = modelHdr.numVerts;
		this->numTris = modelHdr.numTriList;

		// allocate buffers
		Vert_xyzuvn* pCrossData = new Vert_xyzuvn[(unsigned int)modelHdr.numVerts];
		Tri_index* pTriList = new Tri_index[(unsigned int)modelHdr.numTriList];

		// Read verts
		ferror = File::Seek(fh, File::Location::BEGIN, (int)modelHdr.vertBufferOffset);
		assert(ferror == File::Error::SUCCESS);

		ferror = File::Read(fh, pCrossData, sizeof(Vert_xyzuvn) * this->numVerts);
		assert(ferror == File::Error::SUCCESS);

		// Read trilist
		ferror = File::Seek(fh, File::Location::BEGIN, (int)modelHdr.triListBufferOffset);
		assert(ferror == File::Error::SUCCESS);

		ferror = File::Read(fh, pTriList, sizeof(Tri_index) * this->numTris);
		assert(ferror == File::Error::SUCCESS);

		ferror = File::Close(fh);
		assert(ferror == File::Error::SUCCESS);

		// Data is in RAM...
		// Configure and send data to GPU

		// Create a VAO
		glGenVertexArrays(1, &this->vao);
		assert(this->vao != 0);
		glBindVertexArray(this->vao);

		// Create a VBO
		glGenBuffers(1, &this->vbo_verts);
		assert(this->vbo_verts != 0);

		glGenBuffers(1, &this->vbo_trilist);
		assert(this->vbo_trilist != 0);

		// Load the combined data: ---------------------------------------------------------

		glBindBuffer(GL_ARRAY_BUFFER, this->vbo_verts);

		// load the data to the GPU
		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Vert_xyzuvn) * this->numVerts), pCrossData, GL_STATIC_DRAW);

		// Define an array of generic vertex attribute data

		// todo make a table or enum - locations needs enums...

		// Vert data is location: 0  (used in vertex shader)
		void* offsetVert = (void*)((unsigned int)&pCrossData[0].x - (unsigned int)pCrossData);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetVert);
		glEnableVertexAttribArray(0);

		// Texture data is location: 1  (used in vertex shader)
		void* offsetTex = (void*)((unsigned int)&pCrossData[0].u - (unsigned int)pCrossData);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetTex);
		glEnableVertexAttribArray(1);

		// normals data in location 2 (used in vertex shader
		void* offsetNorm = (void*)((unsigned int)&pCrossData[0].nx - (unsigned int)pCrossData);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetNorm);
		glEnableVertexAttribArray(2);

		delete pCrossData;

		// Load the index data: ---------------------------------------------------------

			/* Bind our 2nd VBO as being the active buffer and storing index ) */
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_trilist);

		/* Copy the index data to our buffer */
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Tri_index) * this->numTris), pTriList, GL_STATIC_DRAW);
		delete pTriList;

	}

	CrossModel::CrossModel(const char* const pModelFileName)
		: Model()
	{
		assert(pModelFileName);
		this->privCreateVAO(pModelFileName);
	}

	CrossModel::~CrossModel()
	{
		// remove anything dynamic here
	}


	void CrossModel::SaveToFile()
	{
		//ModelFileHdr hdr;
		//hdr.numTriList = 36;
		//hdr.numVerts = 72;
		//hdr.vertBufferOffset = 48;
		//hdr.triListBufferOffset = hdr.vertBufferOffset + hdr.numVerts * sizeof(Vert_xyzuvn);

		//// File stuff
		//File::Handle fh;
		//File::Error  ferror;

		//ferror = File::Open(fh, "crossModel.azul", File::Mode::WRITE);
		//assert(ferror == File::Error::SUCCESS);

		//ferror = File::Write(fh, &hdr, sizeof(ModelFileHdr));
		//assert(ferror == File::Error::SUCCESS);

		//// Write verts
		//ferror = File::Seek(fh, File::Location::BEGIN, (int)hdr.vertBufferOffset);
		//assert(ferror == File::Error::SUCCESS);

		//ferror = File::Write(fh, &crossData, sizeof(Vert_xyzuvn) * this->numVerts);
		//assert(ferror == File::Error::SUCCESS);

		//// Read trilist
		//ferror = File::Seek(fh, File::Location::BEGIN, (int)hdr.triListBufferOffset);
		//assert(ferror == File::Error::SUCCESS);

		//ferror = File::Write(fh, &triListCross, sizeof(Tri_index) * this->numTris);
		//assert(ferror == File::Error::SUCCESS);

		//ferror = File::Close(fh);
		//assert(ferror == File::Error::SUCCESS);
	}
}

// --- End of File ---
