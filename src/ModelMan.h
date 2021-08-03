//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef MODEL_MAN_H
#define MODEL_MAN_H

#include "Model.h"

namespace Azul
{
	class ModelMan
	{
	public:
		static void Add(Model* pModel);
		static void Create();
		static void Destroy();
		static void SetBone(Model* pBone);
		static Model* GetBone();

	private:  // methods

		static ModelMan* privGetInstance();
		ModelMan();

		void privAddToFront(Model* node, Model*& head);
		void privRemove(Model* node, Model*& head);

	private:  // add

		Model* active;
		Model* pBone;

	};

}

#endif

// ---  End of File ---
