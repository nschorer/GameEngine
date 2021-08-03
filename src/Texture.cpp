//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "sb7.h"
#include "Texture.h"
#include "TextureNode.h"

namespace Azul
{
	Texture::Texture()
		: name(Texture::Name::DEFAULT),
		textureID(0),
		minFilter(GL_LINEAR),
		magFilter(GL_LINEAR),
		horizWrapMode(GL_CLAMP_TO_EDGE),
		vertWrapMode(GL_CLAMP_TO_EDGE),
		width(-1),
		height(-1),
		checksum(0)
	{
		memset(this->assetName, 0, Texture::ASSET_NAME_SIZE);
	}


	void Texture::set(const char* const _assetName,
		Texture::Name _name,
		GLuint _TextureID,
		GLenum _minFilter,
		GLenum _magFilter,
		GLenum _horizWrapMode,
		GLenum _vertWrapMode,
		int _width,
		int _height,
		unsigned int _checksum)
	{
		memset(this->assetName, 0x0, Texture::ASSET_NAME_SIZE);
		memcpy(this->assetName, _assetName, Texture::ASSET_NAME_SIZE - 1);
		this->name = _name;
		this->magFilter = _magFilter;
		this->minFilter = _minFilter;
		this->horizWrapMode = _horizWrapMode;
		this->vertWrapMode = _vertWrapMode;
		this->textureID = _TextureID;
		this->width = _width;
		this->height = _height;
		this->checksum = _checksum;
	}
}

// --- End of File -----

