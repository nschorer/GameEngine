#ifndef FONT_MAN_H
#define FONT_MAN_H

#include "Font.h"
#include "Glyph.h"

namespace Azul
{

	// Singleton
	class FontMan
	{
	public:
		static void Add(Font::Name name, const char* const pMessage, Glyph::Name glyphName);
		static Font* Find(Font::Name name);

		static void Destroy();
		static void Create();

		~FontMan() = default;
		FontMan(const FontMan&) = delete;
		FontMan& operator=(const FontMan&) = delete;


	private:
		FontMan();
		void privAddToFront(FontLink* node, FontLink*& head);
		void privRemove(FontLink* pNode, FontLink*& poHead);
		static FontMan* privGetInstance();

	private:
		FontLink* active;
	};

}

#endif