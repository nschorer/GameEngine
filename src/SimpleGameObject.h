//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef SIMPLE_GAME_OBJECT_H
#define SIMPLE_GAME_OBJECT_H

#include "GameObject.h"
#include "MathEngine.h"
#include "ShaderObject.h"
#include "Model.h"
#include "GraphicsObject.h"
#include "PCSNode.h"

namespace Azul
{
	class SimpleGameObject : public GameObject
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
	public:
		SimpleGameObject(GraphicsObject *graphicsObject);

		// Big four
		SimpleGameObject() = delete;
		SimpleGameObject(const SimpleGameObject &) = delete;
		SimpleGameObject &operator=(SimpleGameObject &) = delete;
		virtual ~SimpleGameObject();

		virtual void Update(AnimTime currentTime);

		void SetScale(float sx, float sy, float sz);
		void SetPos(float x, float y, float z);

		void setMove(MoveDir move);
		void setDeltaRot(float deltaY, float deltaZ);

	private:
		void privUpdate(AnimTime t);

	public:   // add accessors later
		Vect *poScale;
		Vect *poTrans;

		float deltaRotY;
		float deltaRotZ;
		float rotY;
		float rotZ;
		
		MoveDir moveDir;

	};

}

#endif

// --- End of File ---
