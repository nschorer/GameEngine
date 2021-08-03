//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "MathEngine.h"
#include "GraphicsObject.h"
#include "PCSNode.h"
#include "AnimTime.h"

namespace Azul
{

	class GameObject : public PCSNode
	{
	public:
		GameObject() = delete;
		GameObject(const GameObject&) = delete;
		GameObject& operator = (const GameObject&) = delete;
		virtual ~GameObject();

		GameObject(GraphicsObject* graphicsObject);  // Keenan(20)

		virtual void Update(AnimTime currentTime) = 0;
		virtual void Draw();

		// Set / Get:
		GraphicsObject* GetGraphicsObject();
		Matrix* GetWorld();

		// hack functions
		bool IsCollision() const;
		bool IsAnim() const;

		void SetWorld(Matrix* pWorld);

	protected:
		Matrix* poWorld;
		void baseUpdateBoundingSphere();

	protected:
		GraphicsObject* poGraphicsObject;
		bool isCollision = false; // on hacks
		bool isAnim = false; // on hacks
	};
}

#endif


// --- End of File ---
