//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GAME_OBJECT_RIGID_H
#define GAME_OBJECT_RIGID_H

#include "GameObject.h"
#include "MathEngine.h"
#include "GraphicsObject.h"

namespace Azul
{
	// Keenan(33)
	class GameObjectRigid : public GameObject
	{
	public:
		enum MoveDir
		{
			Move_X,
			Move_Y,
			Move_Z,
			Move_None
		};

	public:
		GameObjectRigid(GraphicsObject *graphicsObject);

		// Big four
		GameObjectRigid() = delete;
		GameObjectRigid(const GameObjectRigid &) = delete;
		GameObjectRigid &operator=(GameObjectRigid &) = delete;
		virtual ~GameObjectRigid() override;

		virtual void Update(AnimTime currentTime);

		void SetScale(float sx, float sy, float sz);
		void SetPos(float x, float y, float z);

		void setMove(MoveDir move);
		void setDeltaRot(float deltaY, float deltaZ);

	private:
		void privUpdate(AnimTime currentTime);

	public:
		Vect *pScale;
		Vect *pPos;
		float angle;



		float deltaRotY;
		float deltaRotZ;
		float rotY;
		float rotZ;

		MoveDir moveDir;

	};
}

#endif

// --- End of File ---
