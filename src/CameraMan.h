//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef CAMERA_MAN_H
#define CAMERA_MAN_H

#include "Camera.h"

namespace Azul
{

	class CameraMan
	{

	public:
		static void Add(Camera* pCamera);
		static void Create();
		static void Destroy();
		static Camera* Find(Camera::Name name);
		static void Remove(Camera::Name name);
		static void SetCurrent(Camera::Name name, Camera::Type type);
		static Camera* GetCurrent(Camera::Type type);

	private:  // methods

		static CameraMan* privGetInstance();
		CameraMan();

		void privAddToFront(Camera* node, Camera*& head);
		void privRemove(Camera* node, Camera*& head);

	private:  // add

		Camera* active;
		Camera* pCamOrthographic;
		Camera* pCamPerspective;
	};

}

#endif

// ---  End of File ---
