//----------------------------------------------------------------------------
// Copyright 2021, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef IMAGE_MANAGER_H
#define IMAGE_MANAGER_H

#include "Image.h"

namespace Azul
{
	// Singleton
	class ImageMan
	{
	public:
		static void Add(Image *pModel);
		static void Create();
		static void Destroy();

	private:  // methods

		static ImageMan *privGetInstance();
		ImageMan();

		void privAddToFront(Image *node, Image *&head);
		void privRemove(Image *node, Image *&head);

	private:  // add

		Image *active;

	};
}

#endif

//--- End of File ---
