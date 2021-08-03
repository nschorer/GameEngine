//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GAME_H
#define GAME_H

#include "sb7.h"
#include "Engine.h"
#include "MathEngine.h"
#include "AnimTime.h"
#include "AnimTimer.h"

namespace Azul {
	class Game : public Engine
	{
	public:
		const static int SCREEN_WIDTH = 1200;
		const static int SCREEN_HEIGHT = 900;

	public:
		// constructor
		Game() = delete;
		Game(const Game&) = delete;
		Game& operator=(const Game&) = delete;
		~Game() = default;

		Game(const char* windowName, const int Width, const int Height);

	private:
		virtual void Initialize() override;
		virtual void LoadContent() override;
		virtual void Update(float currentTime) override;
		virtual void Draw() override;
		virtual void UnLoadContent() override;
		virtual void ClearBufferFunc() override;

		// No need for this
		//virtual void onResize(int w, int h) override;

		void ProcessInputs();

	public:

		// Nice and Clean
		AnimTimer	intervalTimer;
		AnimTimer	globalTimer;

	};
}

#endif

// --- End of File ---

