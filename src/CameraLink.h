//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef CAMERA_LINK_H
#define CAMERA_LINK_H

namespace Azul
{
	class CameraLink : public Align16
	{
	public:
		CameraLink()
		{
			this->next = nullptr;
			this->prev = nullptr;
		}

		virtual ~CameraLink()
		{

		}

		CameraLink* next;
		CameraLink* prev;
	};

}

#endif

// ---  End of File ---
