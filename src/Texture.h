//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef TEXTURE_H
#define TEXTURE_H

#include "sb7.h"
#include "TextureLink.h"

namespace Azul
{
	class Texture : public TextureLink
	{
	public:
		static const unsigned int ASSET_NAME_SIZE = 64;

		enum class Name
		{
			DEFAULT,
			STONES,
			RED_BRICK,
			DUCKWEED,
			ROCKS,
			SPACE_FRIGATE,
			ASTRO_BOY,
			BUGGY,
			WAR_BEAR_NORMAL,
			WAR_BEAR_POLAR,
			ROBO,
			INVADERS,
			CONSOLAS36PT,
			TEDDY
		};

	public:
		Texture();

		void set(const char* const _assetName,
			Texture::Name _name,
			GLuint _TextureID,
			GLenum minFilter,
			GLenum magFilter,
			GLenum horizWrapMode,
			GLenum vertWrapMode,
			int _width,
			int _height,
			unsigned int checksum);

		virtual ~Texture() = default;
	private:
		char assetName[Texture::ASSET_NAME_SIZE];

	public:
		Texture::Name name;
		GLuint textureID;
		GLenum minFilter;
		GLenum magFilter;
		GLenum horizWrapMode;
		GLenum vertWrapMode;
		int width;
		int height;
		unsigned int checksum;
	};

}

#endif

// --- End of File --------------------------------------------------
