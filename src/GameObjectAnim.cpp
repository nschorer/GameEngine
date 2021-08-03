//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Game.h"
#include "MathEngine.h"
#include "GameObjectAnim.h"
#include "GameObject.h"
#include "GraphicsObject.h"
#include "Model.h"
#include "Anim.h"
#include "CustomModel.h"

//#define DEBUG_LOCAL
//#define DEBUG_WORLD

namespace Azul
{
	GameObjectAnim::GameObjectAnim(GraphicsObject *pGraphicsObject,  Skeleton* _pSkeleton)
		: GameObject(pGraphicsObject),
		pSkeleton(_pSkeleton)
	{
		this->isAnim = true; // haaaaaaaaaaaack

		assert(pGraphicsObject);
		assert(pSkeleton);

		this->poDof = new Vect(0.0f, 0.0f, 0.0f);
		assert(poDof);

		this->poUp = new Vect(0.0f, 1.0f, 0.0f);
		assert(poUp);

		this->poScale = new Vect(1.0f, 1.0f, 1.0f);
		assert(poScale);

		this->poPos = new Vect(0.0f, 0.0f, 0.0f);
		assert(poPos);

		this->indexBoneArray = 0;

		this->poLocal = new Matrix(Matrix::Special::Identity);
		assert(poLocal);

		this->poBoneOrientation = new Matrix(Matrix::Special::Identity);
		assert(poBoneOrientation);
	}

	GameObjectAnim::~GameObjectAnim()
	{
		delete this->poDof;   
		delete this->poUp;    
		delete this->poScale; 
		delete this->poPos;   

		delete this->poLocal; 
		delete this->poBoneOrientation;
	}

	void GameObjectAnim::Update(AnimTime currentTime)
	{
		// Goal: update the world matrix
		//this->privUpdate(currentTime);
		// Update the skin
		if (pSkeleton->pModel != nullptr) {
			pSkeleton->pModel->poBone[indexBoneArray] = *this->poWorld;
			// delete the tail bone in the teddy
		}


		// update the bounding volume based on world matrix
		this->baseUpdateBoundingSphere();

		// push to graphics object
		Matrix mTmp = *this->poBoneOrientation;

		// push to graphics object
		this->poGraphicsObject->SetWorld(mTmp);
	}

	void GameObjectAnim::SetIndex(int val)
	{
		this->indexBoneArray = val;
	}

	void GameObjectAnim::SetBoneOrientation(const Matrix &tmp)
	{
		*this->poBoneOrientation = tmp;
	}

	Matrix GameObjectAnim::GetBoneOrientation(void) const
	{
		return Matrix(*this->poBoneOrientation);
	}
}

// --- End of File ---
