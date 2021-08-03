//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "GraphicsObject.h"
#include "MathEngine.h"
#include "Model.h"

namespace Azul
{

	GraphicsObject::GraphicsObject(const Model* const _pModel, const ShaderObject* const _pShaderObj)
		: pModel(_pModel),
		pShaderObj(_pShaderObj)
	{
		this->poWorld = new Matrix(Matrix::Special::Identity);
		this->poCurrSphere = new Sphere();
		assert(this->poCurrSphere);
		assert(this->poWorld);
		assert(_pModel);
		assert(_pShaderObj);

	}

	GraphicsObject::~GraphicsObject()
	{
		delete this->poWorld;
		delete this->poCurrSphere;
	}

	void GraphicsObject::Render()
	{
		this->SetState();
		this->SetDataGPU();
		this->Draw();
		this->RestoreState();
	}

	const Model* GraphicsObject::GetModel() const
	{
		return this->pModel;
	}

	Matrix& GraphicsObject::GetWorld()
	{
		return *this->poWorld;
	}

	Sphere* GraphicsObject::GetBoundingSphereForCollision()
	{
		this->privUpdateBoundingSphereForCollision();
		return this->poCurrSphere;
	}

	void GraphicsObject::privUpdateBoundingSphereForCollision()
	{
		// Original model sphere
		Sphere* pTmp = this->pModel->poRefSphere;

		// Update... cntr process through the world
		// Assuming a uniform scaling
		Vect A = pTmp->cntr;

		Vect radius(1.0f, 0.0f, 0.0f);
		Vect B = A + radius;

		Vect A_out = A * (*this->poWorld);
		Vect B_out = B * (*this->poWorld);
		poCurrSphere->rad = (B_out - A_out).mag() * pTmp->rad;
		poCurrSphere->cntr = A_out;
	}

	void GraphicsObject::SetWorld(Matrix& _world)
	{
		*this->poWorld = _world;
	}

}

// ---  End of File ---
