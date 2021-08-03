#ifndef FONT_H
#define FONT_H

#include "FontLink.h"
#include "Glyph.h"

namespace Azul
{

	class Font : public FontLink
	{
	public:
		static const unsigned int FONT_SIZE = 32;

		enum class Name
		{
			TestMessage1,
			TestMessage2,
			TestMessage3
		};

		Font(Font::Name name, const char* const pMessage, Glyph::Name glyphName);

		Font() = delete;
		~Font() = default;

		void UpdateMessage(const char* const pMessage);
		

	public:
		Name name;
		Glyph::Name glyphName;
		char pMessage[FONT_SIZE];
	};

}

#endif