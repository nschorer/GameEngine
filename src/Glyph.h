#ifndef GLYPH_H
#define GLYPH_H

#include "Texture.h"
#include "Rect.h"
#include "GlyphLink.h"

namespace Azul
{

	class Glyph : public GlyphLink
	{
	public:
		enum class Name
		{
			Consoloas36pt,
			Uninitialized
		};

		Glyph(Name name, int key, Texture::Name textName, float x, float y, float width, float height);
		~Glyph() = default;
		Glyph(const Glyph&) = default;
		Glyph& operator=(const Glyph&) = default;

		Name GetName();
		int GetKey();
		Texture* GetTexture();
		Rect& GetSubRect();

	private:
		Rect SubRect;
		Name name;
		int key;
		Texture* pTexture;
	};

}

#endif