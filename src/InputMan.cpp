//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "InputMan.h"

namespace Azul
{

	InputMan* InputMan::poInputMan = nullptr;

	void InputMan::Create()
	{
		InputMan::privCreate();
	}

	void InputMan::Destroy()
	{
		InputMan* pMan = InputMan::privInstance();
		assert(pMan);

		delete pMan->poInputMan;

	}

	InputMan::InputMan()
	{
		this->poKeyboard = new Keyboard();
		assert(this->poKeyboard);

		this->poMouse = new Mouse();
		assert(this->poMouse);
	}

	void InputMan::privCreate()
	{
		InputMan::poInputMan = new InputMan();
		assert(poInputMan);
	}

	InputMan::~InputMan()
	{
		delete this->poKeyboard;
		delete this->poMouse;
	}

	Keyboard* InputMan::GetKeyboard()
	{
		InputMan* pMan = InputMan::privInstance();
		return pMan->poKeyboard;
	}

	Mouse* InputMan::GetMouse()
	{
		InputMan* pMan = InputMan::privInstance();
		return pMan->poMouse;
	}

	InputMan* InputMan::privInstance()
	{
		assert(poInputMan);
		return poInputMan;
	}
}

// --- End of File ---
