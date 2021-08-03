//----------------------------------------------------------------------------
// Copyright 2021, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "SSBO.h"


SSBO::SSBO() 
	:
	ssbo(),
	elementCount(0), 
	elementSize(0),
	poBuffer(nullptr)
{
	glGenBuffers(1, &ssbo);
}


SSBO::~SSBO()
{
	this->poBuffer = nullptr;

	// Tell GPU it can be released
	glDeleteBuffers(1, &ssbo);
}

void SSBO::Set(unsigned _elementCount, unsigned _size, void* pData)
{
	this->elementCount = _elementCount;
	this->elementSize = _size;
	this->poBuffer = pData;

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, (GLsizeiptr)(elementCount * this->elementSize), this->poBuffer, GL_STATIC_DRAW);
}

unsigned int SSBO::GetLocation() const
{
	return this->ssbo;
}

void SSBO::Bind(unsigned int bindLocation)
{
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindLocation, this->ssbo);
}

void *SSBO::Map(SSBO::Access accessType)
{
	void *result = glMapNamedBuffer(this->ssbo, GLenum(accessType));
	assert(result);

	return result;
}

void SSBO::Unmap()
{
	glUnmapNamedBuffer(this->ssbo);
}

void SSBO::CopyToSSBO(SSBO* pOther)
{
	glBindBuffer(GL_COPY_READ_BUFFER, this->ssbo);
	glBindBuffer(GL_COPY_WRITE_BUFFER, pOther->ssbo);

	GLint size = 0;
	glGetBufferParameteriv(GL_COPY_READ_BUFFER, GL_BUFFER_SIZE, &size);
	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, size);
}

//--- End of File ----
