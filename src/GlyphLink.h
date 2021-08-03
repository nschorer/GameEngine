
#ifndef GLYPH_LINK_H
#define GLYPH_LINK_H

namespace Azul
{
	class GlyphLink
	{
	public:
		GlyphLink()
		{
			this->next = nullptr;
			this->prev = nullptr;
		}

		virtual ~GlyphLink()
		{

		}

		GlyphLink* next;
		GlyphLink* prev;
	};
}
#endif