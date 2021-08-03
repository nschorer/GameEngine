//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GAME_MAN_H
#define GAME_MAN_H

#include "Game.h"

namespace Azul
{
	class GameMan
	{
	public:		
		GameMan() = delete;
		GameMan(const GameMan &) = delete;
		GameMan &operator = (const GameMan &) = delete;

		static void Create(const char *windowName, const int Width, const int Height);
		static void Destroy();

		static Game *GetGame();

	private:
		GameMan(Game *pGame);
		~GameMan();

		static GameMan *pInstance;
		static void privCreate(const char *windowName, const int Width, const int Height);
			
		static GameMan *privInstance();
		// Data
		Game *poGame;

	};

}


#endif

// --- End of File ---
