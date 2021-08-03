//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------
#include "CameraMan.h"

namespace Azul
{
	CameraMan::CameraMan()
	{
		this->active = nullptr;
		this->pCamPerspective = nullptr;
		this->pCamOrthographic = nullptr;
	}

	CameraMan* CameraMan::privGetInstance()
	{
		// This is where its actually stored (BSS section)
		static CameraMan shaderMan;
		return &shaderMan;
	}

	void CameraMan::Create()
	{
		CameraMan* pCameraMan;
		pCameraMan = CameraMan::privGetInstance();
		assert(pCameraMan);
	}

	void CameraMan::Destroy()
	{
		CameraMan* pCameraMan = CameraMan::privGetInstance();
		assert(pCameraMan);

		Camera* pLink = pCameraMan->active;

		while (pLink != nullptr)
		{
			Camera* pTmp = pLink;
			pLink = (Camera*)pLink->next;
			pCameraMan->privRemove(pTmp, pCameraMan->active);
			delete pTmp;
		}

	}

	Camera* CameraMan::Find(Camera::Name name)
	{
		CameraMan* pCameraMan = CameraMan::privGetInstance();
		assert(pCameraMan);

		Camera* pLink = pCameraMan->active;
		Camera::Name temp = Camera::Name::NOT_INITIALIZED;

		while (pLink != nullptr)
		{
			pLink->getName(temp);
			if (temp == name) {
				return pLink;
			}
			pLink = (Camera*)pLink->next;
			
		}

		return nullptr;
	}

	void CameraMan::Remove(Camera::Name name)
	{
		CameraMan* pCameraMan = CameraMan::privGetInstance();
		assert(pCameraMan);

		Camera* pCamera = Find(name);
		assert(pCamera);

		pCameraMan->privRemove(pCamera, pCameraMan->active);
		delete pCamera;
	}

	void CameraMan::Add(Camera* pCamera)
	{
		// Get the instance to the manager
		CameraMan* pCameraMan = CameraMan::privGetInstance();
		assert(pCameraMan);

		// Create a Camera
		assert(pCamera);

		// Now add it to the manager
		pCameraMan->privAddToFront(pCamera, pCameraMan->active);
	}

	void CameraMan::SetCurrent(const Camera::Name name, Camera::Type type)
	{
		CameraMan* pCamMan = CameraMan::privGetInstance();
		assert(pCamMan);

		Camera* pCam = CameraMan::Find(name);
		assert(pCam);

		switch (type)
		{
		case Camera::Type::PERSPECTIVE_3D:
			assert(pCam->getType() == Camera::Type::PERSPECTIVE_3D);
			pCamMan->pCamPerspective = pCam;
			break;

		case Camera::Type::ORTHOGRAPHIC_2D:
			assert(pCam->getType() == Camera::Type::ORTHOGRAPHIC_2D);
			pCamMan->pCamOrthographic = pCam;
			break;

		default:
			assert(false);
			break;
		}
	}

	Camera* CameraMan::GetCurrent(Camera::Type type)
	{
		Camera* pCam = 0;
		switch (type)
		{
		case Camera::Type::PERSPECTIVE_3D:
			pCam = CameraMan::privGetInstance()->pCamPerspective;
			break;

		case Camera::Type::ORTHOGRAPHIC_2D:
			pCam = CameraMan::privGetInstance()->pCamOrthographic;
			break;

		default:
			assert(false);
			break;
		}

		assert(pCam != 0);
		return pCam;
	}

	void CameraMan::privAddToFront(Camera* node, Camera*& head)
	{
		assert(node != 0);

		if (head == 0)
		{
			head = node;
			node->next = 0;
			node->prev = 0;
		}
		else
		{
			node->next = head;
			head->prev = node;
			head = node;
		}
	}

	void CameraMan::privRemove(Camera* pNode, Camera*& poHead)
	{
		assert(pNode);

		if (pNode->prev != nullptr)
		{	// middle or last node
			pNode->prev->next = pNode->next;
		}
		else
		{  // first
			poHead = (Camera*)pNode->next;
		}

		if (pNode->next != nullptr)
		{	// middle node
			pNode->next->prev = pNode->prev;
		}

	}

}

// ---  End of File ---
