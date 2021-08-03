//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GAME_OBJECT_ANIM_H
#define GAME_OBJECT_ANIM_H

#include "GameObject.h"
#include "MathEngine.h"
#include "GraphicsObject.h"
#include "Bone.h"

namespace Azul
{
	class Skeleton;

	class GameObjectAnim : public GameObject
	{
	public:
		GameObjectAnim(GraphicsObject *graphicsObject, Skeleton* pSkeleton);

		// Big four
		GameObjectAnim() = delete;
		GameObjectAnim(const GameObjectAnim &) = delete;
		GameObjectAnim &operator=(GameObjectAnim &) = delete;
		virtual ~GameObjectAnim() override;

		virtual void Update(AnimTime currentTime);

		// Accessors
		Matrix GetBoneOrientation(void) const;
		void SetBoneOrientation(const Matrix &);
		void SetIndex(int val);

		Skeleton* pSkeleton;

	public:
		//Skeleton* pSkeleton; // back reference
		
		Vect *poScale;
		Vect *poPos;
		Vect *poDof;
		Vect *poUp;
	
		Matrix *poLocal;
		Matrix *poBoneOrientation;
		int    indexBoneArray;
	};

}

#endif

// --- End of File ---
