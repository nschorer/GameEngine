

#include "MathEngine.h"
#include "GameObjectFont.h"
#include "GraphicsObject.h"

namespace Azul
{
	GameObjectFont::GameObjectFont(GraphicsObject_Font* pGraphicsObject)
		: GameObject(pGraphicsObject)
	{
		assert(pGraphicsObject != 0);

		this->angle = 0.0f;
		this->posX = pGraphicsObject->origPosX;
		this->posY = pGraphicsObject->origPosY;
	}

	GameObjectFont::~GameObjectFont()
	{
	}

	void GameObjectFont::Draw()
	{
		GraphicsObject_Font* pGraphicsObj = (GraphicsObject_Font*)this->GetGraphicsObject();
		assert(pGraphicsObj);
		while (pGraphicsObj->SetLetter(this->posX, this->posY))
		{
			pGraphicsObj->Render();
		}
	}


	void GameObjectFont::Update(AnimTime currentTime)
	{
		AZUL_UNUSED_VAR(currentTime);

		// Goal: update the world matrix
		Matrix Scale(Matrix::Scale::XYZ, 1.0f, 1.0f, 1.0f);
		Matrix Rot(Matrix::Rot1::Z, this->angle);
		Matrix Trans(Matrix::Trans::XYZ, this->posX, this->posY, 0.0f);

		*this->poWorld = Scale * Rot * Trans;

		// push to graphics object
		this->poGraphicsObject->SetWorld(*this->poWorld);
	}

}

//--- End of File ---
