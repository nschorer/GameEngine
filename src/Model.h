//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef MODEL_H
#define MODEL_H

#include "sb7.h"
#include "ModelLink.h"
#include "BoundingSphere.h"
#include "TextureMan.h"

namespace Azul
{


	class Model : public ModelLink
	{
	public:
		Model();
		Model(Model& copyModel) = delete;
		Model& operator = (Model& copyModel) = delete;
		virtual ~Model();


		Texture::Name GetTexture() const;

		// Data
		int numVerts;
		int numTris;    // Keenan(20)

		GLuint vao;
		GLuint vbo_verts;   // xyzuvn
		GLuint vbo_trilist;   // trilist
		GLuint vbo_boneweight;   // Bone_weight
		GLuint vbo_boneindex;   // Bone_index

		Sphere* poRefSphere;

		// Make these private
		Matrix* poInversePose;
		Matrix* poBone;
		unsigned int numBones;

	protected:
		// load VAO
		virtual void privCreateVAO(const char* const pModelFileName) = 0;

		Texture::Name textName;

	private:

		

	};

}

#endif

// --- End of File ---
