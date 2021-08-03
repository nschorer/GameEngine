//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "MathEngine.h"
#include "GameObjectRigid.h"
#include "GraphicsObject.h"

namespace Azul
{
	GameObjectRigid::GameObjectRigid(GraphicsObject *pGraphicsObject)
		: GameObject(pGraphicsObject)
	{
		assert(pGraphicsObject != 0);

		this->pScale = new Vect(1.0f, 1.0f, 1.0f);
		assert(this->pScale);

		this->pPos = new Vect(0.0f, 0.0f, 0.0f);
		assert(this->pPos);

		this->angle = 0.0f;

		this->rotY = 0.0f;
		this->rotZ = 0.0f;
		this->deltaRotZ = 0.0f;
		this->deltaRotY = 0.0f;
	}

	GameObjectRigid::~GameObjectRigid()
	{
		delete this->pPos;
		delete this->pScale;
	}

	void GameObjectRigid::privUpdate(AnimTime currentTime)
	{
		// for Humanoid
		/*Vect Trans = Vect(-280.172638f, 1288.917847f - 500, 526.212402f);
		Matrix T = Matrix(Matrix::Trans::XYZ, -Trans);
		Matrix M = T;
		Matrix R = Matrix(Matrix::Rot1::Z, 0);
		Matrix S = Matrix(Matrix::Scale::XYZ, 0.5, 0.5, 0.5);

		*this->poWorld = M * R;*/


		// Special transform for Teddy
		//Matrix Rx = Matrix(Matrix::Rot1::X, MATH_PI2);
		//Matrix Rz = Matrix(Matrix::Rot1::Z, 1.7f);
		//Matrix M = Rx * Rz;

		//*this->poWorld = M;

		AZUL_UNUSED_VAR(currentTime);

		float delta = 0.01f;

		static float dirX = 1.0f;
		static float dirY = 1.0f;
		static float dirZ = 1.0f;

		switch (this->moveDir)
		{
		case Move_X:
			this->pPos->x() += (delta * dirX);
			if (this->pPos->x() > 6.0f || this->pPos->x() < -6.0f)
			{
				dirX *= -1.0f;
			}
			break;

		case Move_Y:
			this->pPos->y() += (delta * dirY);
			if (this->pPos->y() > 4.0f || this->pPos->y() < -4.0f)
			{
				dirY *= -1.0f;
			}
			break;

		case Move_Z:
			this->pPos->z() += (delta * dirZ);
			if (this->pPos->z() > 7.0f || this->pPos->z() < -5.0f)
			{
				dirZ *= -1.0f;
			}
			break;

		case Move_None:
		default:
			break;
		}

		// Goal: update the world matrix

		this->rotY += deltaRotY;
		this->rotZ += deltaRotZ;

		Matrix Scale(Matrix::Scale::XYZ, *this->pScale);
		Matrix TransA(Matrix::Trans::XYZ, *this->pPos);
		Matrix RotY(Matrix::Rot1::Y, this->rotY);
		Matrix RotZ(Matrix::Rot1::Z, this->rotZ);

		// world matrix
		*this->poWorld = Scale * RotY * RotZ * TransA;
	}

	void GameObjectRigid::Update(AnimTime currentTime)
	{
		// Goal: update the world matrix
		this->privUpdate(currentTime);

		// update the bounding volume based on world matrix
		this->baseUpdateBoundingSphere();

		// push to graphics object
		this->poGraphicsObject->SetWorld(*this->poWorld);
	}
	void GameObjectRigid::SetScale(float sx, float sy, float sz)
	{
		this->pScale->set(sx, sy, sz);
	}
	void GameObjectRigid::SetPos(float x, float y, float z)
	{
		this->pPos->set(x, y, z);
	}
	void GameObjectRigid::setMove(MoveDir move)
	{
		this->moveDir = move;
	}
	void GameObjectRigid::setDeltaRot(float deltaY, float deltaZ)
	{
		this->deltaRotY = deltaY;
		this->deltaRotZ = deltaZ;
	}
}

// --- End of File ---
