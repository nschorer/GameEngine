//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Model.h"

namespace Azul
{
	Model::Model()
		: numVerts(0),
		numTris(0),    // Keenan(20)
		vao(0),
		vbo_verts(0),
		vbo_trilist(0),
		vbo_boneweight(0),
		vbo_boneindex(0),
		poRefSphere(new Sphere()),
		poInversePose(nullptr), poBone(nullptr), numBones(0),
		textName(Texture::Name::DEFAULT)
	{
	}

	Model::~Model()
	{
		delete this->poRefSphere;
	}
	Texture::Name Model::GetTexture() const
	{
		return this->textName;
	}
}
// --- End of File ---
