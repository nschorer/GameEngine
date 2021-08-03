#include "Glyph.h"
#include "TextureMan.h"

namespace Azul
{

	Glyph::Glyph(Name _name, int _key, Texture::Name textName, float x, float y, float width, float height)
		: SubRect(x, y, width, height), name(_name), key(_key)
	{
		pTexture = TextureMan::Find(textName);
		assert(pTexture != nullptr);

	}

	Glyph::Name Glyph::GetName()
	{
		return this->name;
	}

	int Glyph::GetKey()
	{
		return this->key;
	}

	Texture* Glyph::GetTexture()
	{
		return this->pTexture;
	}

	Rect& Glyph::GetSubRect()
	{
		return this->SubRect;
	}

}
