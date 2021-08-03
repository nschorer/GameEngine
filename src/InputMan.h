//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef INPUT_MAN_H
#define INPUT_MAN_H

#include "Keyboard.h"
#include "Mouse.h"

namespace Azul
{
	class InputMan
	{
	public:
		InputMan(const InputMan&) = delete;
		InputMan& operator = (const InputMan&) = delete;
		~InputMan();

		static void Create();
		static void Destroy();

		static Keyboard* GetKeyboard();
		static Mouse* GetMouse();

	private:
		InputMan();

		static InputMan* privInstance();
		static void privCreate();

		// Data
		static InputMan* poInputMan;
		Keyboard* poKeyboard;
		Mouse* poMouse;

	};

}

#endif

// --- End of File ---
