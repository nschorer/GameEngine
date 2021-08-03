//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "PyramidModel.h"
#include "GraphicsObject_TextureLight.h"
#include "GameObjectMan.h"
#include "ModelMan.h"
#include "ShaderMan.h"
#include "GameObjectRigid.h"
#include "GraphicsObject_Null.h"
#include "NullModel.h"
#include "Skeleton.h"
#include "PCSTreeForwardIterator.h"
#include "GameObjectMan.h"


//#define DEBUG_SKELETON
//#define DEBUG_POSE_GPU_LOCAL
//#define DEBUG_POSE_GPU_WORLD

namespace Azul
{


	Skeleton::Skeleton(SkeletonData* pBoneH, int _numBones)
		: pModel(nullptr),
		pFirstBone(nullptr),
		numBones(_numBones)
	{
		this->privSetAnimationHierarchy(pBoneH);
		assert(pFirstBone);
	}

	GameObjectAnim *Skeleton::GetFirstBone()
	{
		assert(pFirstBone);
		return this->pFirstBone;
	}

	void Skeleton::SetModel(Model* _pModel)
	{
		//assert(pModel != nullptr);
		pModel = _pModel;
	}

	GameObjectAnim *Skeleton::privFindBoneByIndex(int index)
	{
		GameObjectAnim *pFound = nullptr;

		GameObjectAnim *pObj = this->GetFirstBone();

		// TODO - add test bed for an interator of a tree with only one node
		if (pObj->indexBoneArray == index)
		{
			pFound = pObj;
		}
		else
		{
			PCSTreeForwardIterator pIter(pObj);
			PCSNode *pNode = pIter.First();
			GameObjectAnim *pGameObj = nullptr;

			// walks the anim node does the pose for everything that
			while (pNode != nullptr)
			{
				assert(pNode);
				// Update the game object
				pGameObj = (GameObjectAnim *)pNode;
				if (pGameObj->indexBoneArray == index)
				{
					pFound = pGameObj;
					break;
				}
				pNode = pIter.Next();
			}
		}

		// Hack
		if (pFound == nullptr)
		{
			pFound = pObj;
		}
		return pFound;
	}

	void Skeleton::privSetAnimationHierarchy(SkeletonData* pBoneH)
	{
		// Load the model
		Model* pPyramidModel = ModelMan::GetBone();
		assert(pPyramidModel);

		// Create/Load Shader 
		ShaderObject* pShaderObject_textureLight = ShaderMan::GetBone();
		assert(pShaderObject_textureLight);

		ShaderObject* pShaderObject_null = ShaderMan::GetNull();
		assert(pShaderObject_null);

		// GraphicsObject for a specific instance
		GraphicsObject_TextureLight* pGraphics_TextureLight;

		// Create GameObject
		Vect color;
		Vect pos(1, 1, 1);

		// Null object
		NullModel* pNullModel = new NullModel(0);
		ModelMan::Add(pNullModel);
		GraphicsObject_Null* pGraphicsObjNull = new GraphicsObject_Null(pNullModel, pShaderObject_null);
		GameObjectRigid* pGameRigid = new GameObjectRigid(pGraphicsObjNull);
		pGameRigid->SetName("Rigid");
		GameObjectMan::Add(pGameRigid, GameObjectMan::GetRoot());

		// Human from txt file
		color.set(0.0f, 1.0f, 1.0f, 1.0f);

		pGraphics_TextureLight = new GraphicsObject_TextureLight(pPyramidModel, pShaderObject_textureLight, Texture::Name::DUCKWEED, color, pos);
		GameObjectAnim* pObj = new GameObjectAnim(pGraphics_TextureLight, this);
		pObj->SetIndex(pBoneH[0].index);
		pObj->SetName(pBoneH[0].name);
		GameObjectMan::Add(pObj, pGameRigid);
		this->pFirstBone = pObj;



#ifdef DEBUG_SKELETON
		Trace::out("\n%s(CPU): (%i, %i)", pBoneH[0].name, pBoneH[0].index, pBoneH[0].parentIndex);
#endif
		for (int i = 1; i < this->numBones; i++)
		{
				pGraphics_TextureLight = new GraphicsObject_TextureLight(pPyramidModel, pShaderObject_textureLight, Texture::Name::DUCKWEED, color, pos);
				pObj = new GameObjectAnim(pGraphics_TextureLight, this);
				pObj->SetIndex(pBoneH[i].index);
				pObj->SetName(pBoneH[i].name);

				GameObjectAnim* pParent = this->privFindBoneByIndex(pBoneH[i].parentIndex);
				assert(pParent);
				GameObjectMan::Add(pObj, pParent);

#ifdef DEBUG_SKELETON
				Trace::out("\n%s(CPU): (%i, %i)", pBoneH[i].name, pBoneH[i].index, pBoneH[i].parentIndex);
#endif
		}

#ifdef DEBUG_SKELETON
		Trace::out("\n");
#endif

		color.set(0.0f, 0.0f, 0.0f, 1.0f);
	}
}

// --- End of File ---
