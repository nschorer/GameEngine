//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "TextureNode.h"

namespace Azul
{

	TextureNode::TextureNode()
		: name(Texture::Name::DEFAULT),
		textureID(0),
		minFilter(GL_LINEAR),
		magFilter(GL_LINEAR),
		horizWrapMode(GL_CLAMP_TO_EDGE),
		vertWrapMode(GL_CLAMP_TO_EDGE)
	{
		memset(this->assetName, 0, Texture::ASSET_NAME_SIZE);
	}

	void TextureNode::set(const char* const _assetName,
		Texture::Name _name,
		GLuint _TextureID,
		GLenum _minFilter,
		GLenum _magFilter,
		GLenum _horizWrapMode,
		GLenum _vertWrapMode,
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
		this->checksum = _checksum;
	}

}

// --- End of File ---
