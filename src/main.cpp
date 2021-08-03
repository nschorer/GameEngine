//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "GameMan.h"

using namespace Azul;

int CALLBACK WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
	// Game is inside a singleton
	GameMan::Create("Animation", Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT);

	Game* pGame = GameMan::GetGame();
	pGame->Run();

	GameMan::Destroy();

	return 0;
}

// ---  End of File ----------
