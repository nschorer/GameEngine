//----------------------------------------------------------------------------
// Copyright 2021, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "MathEngine.h"
#include "GameObjectNull.h"
#include "GraphicsObject.h"

namespace Azul
{
	// w1(4)
	GameObjectNull::GameObjectNull(GraphicsObject_Null *pGraphicsObject)
		: GameObject(pGraphicsObject)
	{
		assert(pGraphicsObject != nullptr);

	}

	GameObjectNull::~GameObjectNull()
	{
	}


	void GameObjectNull::Update(AnimTime currentTime)
	{
		AZUL_UNUSED_VAR(currentTime);

		// Goal: update the world matrix
		Matrix Identity(Matrix::Special::Identity);
		*this->poWorld = Identity;

		// push to graphics object
		this->poGraphicsObject->SetWorld(*this->poWorld);
	}

}

//--- End of File ------------------------------------------------------------
