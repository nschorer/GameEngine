//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GAME_OBJECT_MAN_H
#define GAME_OBJECT_MAN_H

#include "GameObject.h"
#include "PCSTree.h"
#include "AnimTime.h"

namespace Azul
{

	// Singleton
	class GameObjectMan
	{
	public:
		static void Add(GameObject* pObj);
		static void Add(GameObject* pObj, GameObject* pParent);
		static void Draw(void);
		static void Update(AnimTime currentTime);
		static GameObject* GetRoot(void);
		static PCSTree* GetPCSTree();

		static void Destroy();
		static void Create();
		~GameObjectMan();

		static void ToggleRenderCollision();
		static void ToggleRenderSkeleton();


	private:
		GameObjectMan();

		static GameObjectMan* privGetInstance();

		// data
		PCSTree* poRootTree;

		bool renderCollision = false;
		bool renderSkeleton = true;
	};

}

#endif


// --- End of File ---
