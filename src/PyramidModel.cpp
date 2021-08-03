//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "PyramidModel.h"
#include "File.h"
#include "ModelFileHdr.h"

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

#define TRILIST_NUM_TRIANGLES (sizeof(pyramidTriList)/sizeof(Tri_index))


	Tri_index pyramidTriList[] =
	{
		{0,2,1},
		{2,0,3},
		{4,6,5},
		{5,6,7},
		{8,10,9},
		{9,10,11},
		{12,14,13},
		{13,14,16},
		{16,18,17},
		{18,16,19},
		{20,22,21},
		{22,20,23}
	};



#define PYRAMID_DATA_NUM_VERTS (sizeof(pyramidData)/sizeof(Vert_xyzuvn))



	Vert_xyzuvn  pyramidData[] =
	{
		// Triangle 0
		{-0.25f,  0.0f, -0.25f, 0.0f, 0.0f, -0.6f,  0.6f, -0.6f  }, //0
		{-0.25f, -0.0f, -0.25f, 0.0f, 1.0f, -0.6f, -0.6f, -0.6f  },// 1
		{ 0.25f, -0.0f, -0.25f, 1.0f, 1.0f,  0.6f, -0.6f, -0.6f  }, //2

		// Triangle 1

		{ 0.0f,   1.0f, -0.0f, 1.0f, 0.0f,  0.6f,  0.6f, -0.6f  },  //4  3


		// Triangle 2
		{ 0.25f, -0.0f, -0.25f, 0.0f, 1.0f, 0.6f, -0.6f, -0.6f  }, //6
		{ 0.25f, -0.0f,  0.25f, 1.0f, 1.0f, 0.6f, -0.6f,  0.6f  }, //7
		{ 0.0f,  1.0f, -0.0f, 0.0f, 0.0f, 0.6f,  0.6f, -0.6f  }, //8

		// Triangle 3

		{ 0.25f,  0.0f,  0.25f, 1.0f, 0.0f, 0.6f,  0.6f,  0.6f  }, //9 


		// Triangle 4
		{ 0.25f, -0.0f,  0.25f, 1.0f, 1.0f,  0.6f, -0.6f, 0.6f  },  // 12
		{-0.25f, -0.0f, 0.25f, 0.0f, 1.0f, -0.6f, -0.6f, 0.6f  },   // 13
		{ 0.0f,  1.0f,  0.0f, 1.0f, 0.0f,  0.6f,  0.6f, 0.6f  },  // 14

		// Triangle 5

		{ -0.0f,  1.0f,  0.0f, 0.0f, 0.0f, -0.6f,  0.6f, 0.6f },  //15


		// Triangle 6
		{-0.25f, -0.0f,  0.25f, 0.0f, 0.0f, -0.6f, -0.6f,  0.6f }, //18
		{-0.25f, -0.0f, -0.25f, 0.0f, 1.0f, -0.6f, -0.6f, -0.6f }, //19
		{-0.0f,  1.0f,  0.0f, 1.0f, 0.0f, -0.6f,  0.6f,  0.6f }, //20

		// Triangle 7

		{-0.0f,  1.0f, -0.0f, 1.0f, 1.0f, -0.6f,  0.6f, -0.6f }, // 21


		// Triangle 8
		{-0.25f, -0.0f,  0.25f, 0.0f, 0.0f, -0.6f, -0.6f,  0.6f }, //24
		{0.25f, -0.0f,   0.25f, 1.0f, 0.0f,  0.6f, -0.6f,  0.6f }, //25
		{0.25f, -0.0f,  -0.25f, 1.0f, 1.0f,  0.6f, -0.6f, -0.6f }, //26

		// Triangle 9

		{-0.25f, -0.0f, -0.25f, 0.0f, 1.0f, -0.6f, -0.6f, -0.6f}, // 27


		// Triangle 10
		{-0.0f,  1.0f, -0.0f, 0.0f, 1.0f, -0.6f, 0.6f, -0.6f }, //30
		{0.0f,  1.0f,  -0.0f, 1.0f, 1.0f,  0.6f, 0.6f, -0.6f }, //31
		{0.0f,  1.0f,   0.0f, 1.0f, 0.0f,  0.6f, 0.6f,  0.6f }, //32

		// Triangle 11

		{-0.0f,  1.0f,  0.0f, 0.0f, 0.0f, -0.6f, 0.6f,  0.6f }, //33

	};


	void PyramidModel::privCreateVAO(const char* const pModelFileName)
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
		Vert_xyzuvn* pPyramidData = new Vert_xyzuvn[(unsigned int)modelHdr.numVerts];
		Tri_index* pTriList = new Tri_index[(unsigned int)modelHdr.numTriList];

		// Read verts
		ferror = File::Seek(fh, File::Location::BEGIN, (int)modelHdr.vertBufferOffset);
		assert(ferror == File::Error::SUCCESS);

		ferror = File::Read(fh, pPyramidData, sizeof(Vert_xyzuvn) * this->numVerts);
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
		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Vert_xyzuvn) * this->numVerts), pPyramidData, GL_STATIC_DRAW);

		// Define an array of generic vertex attribute data

		// todo make a table or enum - locations needs enums...

		// Vert data is location: 0  (used in vertex shader)
		void* offsetVert = (void*)((unsigned int)&pPyramidData[0].x - (unsigned int)pPyramidData);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetVert);
		glEnableVertexAttribArray(0);

		// Texture data is location: 1  (used in vertex shader)
		void* offsetTex = (void*)((unsigned int)&pPyramidData[0].u - (unsigned int)pPyramidData);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetTex);
		glEnableVertexAttribArray(1);

		// normals data in location 2 (used in vertex shader
		void* offsetNorm = (void*)((unsigned int)&pPyramidData[0].nx - (unsigned int)pPyramidData);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetNorm);
		glEnableVertexAttribArray(2);

		delete pPyramidData;

		// Load the index data: ---------------------------------------------------------

			/* Bind our 2nd VBO as being the active buffer and storing index ) */
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_trilist);

		/* Copy the index data to our buffer */
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Tri_index) * this->numTris), pTriList, GL_STATIC_DRAW);
		delete pTriList;
	}

	void PyramidModel::privCreateVAO()
	{
		// future proofing it for a file
		//AZUL_UNUSED_VAR(modelFileName);

		this->numVerts = PYRAMID_DATA_NUM_VERTS;
		this->numTris = TRILIST_NUM_TRIANGLES;

		Matrix M(Matrix::Rot1::X, MATH_PI2);
		for (int i = 0; i < this->numVerts; i++)
		{
			Vect v(pyramidData[i].x, pyramidData[i].y, pyramidData[i].z);
			v = v * M;
			pyramidData[i].x = v[x];
			pyramidData[i].y = v[y];
			pyramidData[i].z = v[z];
		}

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
		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Vert_xyzuvn) * this->numVerts), pyramidData, GL_STATIC_DRAW);

		// Vertext data in location 0
		void* offsetVert = (void*)((unsigned int)&pyramidData[0].x - (unsigned int)pyramidData);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetVert);
		glEnableVertexAttribArray(0);

		// Texture data in location 1
		void* offsetTex = (void*)((unsigned int)&pyramidData[0].u - (unsigned int)pyramidData);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetTex);
		glEnableVertexAttribArray(1);

		// Normal data in location 2
		void* offsetNorm = (void*)((unsigned int)&pyramidData[0].nx - (unsigned int)pyramidData);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetNorm);
		glEnableVertexAttribArray(2);


		// Load the index data: ---------------------------------------------------------

			  /* Bind our 2nd VBO as being the active buffer and storing index ) */
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_trilist);

		/* Copy the index data to our buffer */
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Tri_index) * this->numTris), pyramidTriList, GL_STATIC_DRAW);
	}

	PyramidModel::PyramidModel(const char* const)
		: Model()
	{
		//assert(pModelFileName);
		//this->privCreateVAO(pModelFileName);
		this->privCreateVAO();
	}

	PyramidModel::~PyramidModel()
	{
		// remove anything dynamic here
	}

	void PyramidModel::SaveToFile()
	{
		//ModelFileHdr hdr;
		//hdr.numTriList = 36;
		//hdr.numVerts = 72;
		//hdr.vertBufferOffset = 48;
		//hdr.triListBufferOffset = 72 * sizeof(Vert_xyzuvn);

		//// File stuff
		//File::Handle fh;
		//File::Error  ferror;

		//ferror = File::Open(fh, "pyramidModel.azul", File::Mode::WRITE);
		//assert(ferror == File::Error::SUCCESS);

		//ferror = File::Write(fh, &hdr, sizeof(ModelFileHdr));
		//assert(ferror == File::Error::SUCCESS);

		//// Write verts
		//ferror = File::Seek(fh, File::Location::BEGIN, (int)hdr.vertBufferOffset);
		//assert(ferror == File::Error::SUCCESS);

		//ferror = File::Write(fh, &pyramidData, sizeof(Vert_xyzuvn) * this->numVerts);
		//assert(ferror == File::Error::SUCCESS);

		//// Read trilist
		//ferror = File::Seek(fh, File::Location::BEGIN, (int)hdr.triListBufferOffset);
		//assert(ferror == File::Error::SUCCESS);

		//ferror = File::Write(fh, &triListPyramid, sizeof(Tri_index) * this->numTris);
		//assert(ferror == File::Error::SUCCESS);

		//ferror = File::Close(fh);
		//assert(ferror == File::Error::SUCCESS);
	}
}

// --- End of File ---
