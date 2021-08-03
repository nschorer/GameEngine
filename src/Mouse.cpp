//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "GameMan.h"
#include "Mouse.h"

namespace Azul
{

	Mouse::Mouse()
	{

	}

	// Use this to read mouse buttons
	bool Mouse::GetKeyState(Button button)
	{
		bool value;
		Game* pGame = GameMan::GetGame();

		if (pGame->GetMouseButton(button) == GLFW_PRESS)
		{
			value = true;
		}
		else
		{
			value = false;
		}
		return value;
	}

	void Mouse::GetCursor(float& xPos, float& yPos)
	{
		Game* pGame = GameMan::GetGame();
		// get mouse position
		pGame->GetCursorPos(xPos, yPos);

	}

}

// --- End of File ---