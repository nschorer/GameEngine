#ifndef GLYPH_MAN_H
#define GLYPH_MAN_H

#include "Glyph.h"
#include "Texture.h"

namespace Azul
{

	// Singleton
	class GlyphMan
	{
	public:
		static void AddXML(Glyph::Name glyphName, const char* const assetName, Texture::Name textName);
		static void Add(Glyph::Name name, int key, Texture::Name textName, float x, float y, float width, float height);
		static Glyph* Find(Glyph::Name name, int key);

		static void Destroy();
		static void Create();

		~GlyphMan() = default;
		GlyphMan(const GlyphMan&) = delete;
		GlyphMan& operator=(const GlyphMan&) = delete;



	private:
		GlyphMan();
		static GlyphMan* privGetInstance();

		void privAddToFront(GlyphLink* node, GlyphLink*& head);
		void privRemove(GlyphLink* pNode, GlyphLink*& poHead);

	private:
		GlyphLink* active;
	};

}

#endif