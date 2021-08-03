#include "Font.h"

namespace Azul {
	Font::Font(Font::Name _name, const char* const _pMessage, Glyph::Name _glyphName)
		: name(_name), glyphName(_glyphName)
	{
		this->UpdateMessage(_pMessage);
	}

	void Font::UpdateMessage(const char* const _pMessage)
	{
		strcpy_s(pMessage, FONT_SIZE, _pMessage);
	}

}
