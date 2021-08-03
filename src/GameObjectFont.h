#ifndef GAME_OBJECT_FONT_H
#define GAME_OBJECT_FONT_H

#include "MathEngine.h"
#include "GameObject.h"
#include "GraphicsObject_Font.h"

namespace Azul
{
	class GameObjectFont : public GameObject
	{
	public:

		GameObjectFont(GraphicsObject_Font* graphicsObject);

		// Big four
		GameObjectFont() = delete;
		GameObjectFont(const GameObjectFont&) = delete;
		GameObjectFont& operator=(GameObjectFont&) = delete;
		virtual ~GameObjectFont() override;

		virtual void Draw() override;

		virtual void Update(AnimTime currentTime) override;


	public:
		float posX;
		float posY;
		float angle;
	};
}

#endif