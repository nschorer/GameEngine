//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include <math.h>
#include "MathEngine.h"
#include "GameObject.h"
#include "Game.h"

namespace Azul
{

	GameObject::GameObject(GraphicsObject* pGraphicsObject)
		: poGraphicsObject(pGraphicsObject)
	{
		this->poWorld = new Matrix(Matrix::Special::Identity);
		assert(poWorld);
		assert(pGraphicsObject);
	}

	GameObject::~GameObject()
	{
		delete this->poWorld;
		delete this->poGraphicsObject;
	}

	Matrix* GameObject::GetWorld()
	{
		return this->poWorld;
	}

	bool GameObject::IsCollision() const
	{
		return isCollision;
	}

	bool GameObject::IsAnim() const
	{
		return isAnim;
	}

	GraphicsObject* GameObject::GetGraphicsObject()
	{
		return this->poGraphicsObject;
	}

	void GameObject::SetWorld(Matrix* pWorld)
	{
		assert(pWorld);
		*this->poWorld = *pWorld;
	}

	void GameObject::baseUpdateBoundingSphere()
	{
		// Get the ref Sphere
		Sphere* pRefSphere = poGraphicsObject->GetModel()->poRefSphere;
		assert(pRefSphere);

		// Get the curr Sphere
		Sphere* pCurrSphere = poGraphicsObject->poCurrSphere;
		assert(pCurrSphere);

		// Update... cntr process through the world
		// Assuming a uniform scaling
		Vect A = pRefSphere->cntr;
		Vect radius(1.0f, 0.0f, 0.0f);
		Vect B = A + radius;
		Vect A_out = A * (*this->poWorld);
		Vect B_out = B * (*this->poWorld);

		pCurrSphere->rad = (B_out[x] - A_out[x]) * pRefSphere->rad;
		pCurrSphere->cntr = A_out;
	}

	void GameObject::Draw()
	{
		GraphicsObject* pGraphicsObj = this->GetGraphicsObject();
		assert(pGraphicsObj);

		pGraphicsObj->Render();
	}

}

// --- End of File ---
