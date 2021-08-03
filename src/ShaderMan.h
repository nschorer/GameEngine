//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef SHADER_MAN_H
#define SHADER_MAN_H

#include "ShaderObject.h"

namespace Azul
{
	class ShaderMan
	{
	public:
		static void Add(ShaderObject* pShader);
		static void Create();
		static void Destroy();
		static ShaderObject* Find(ShaderObject::Name name);

		// I should make proper enums for these
		static void SetBone(ShaderObject* pShader);
		static ShaderObject* GetBone();
		static void SetNull(ShaderObject* pShader);
		static ShaderObject* GetNull();

	private:  // methods

		static ShaderMan* privGetInstance();
		ShaderMan();

		void privAddToFront(ShaderObject* node, ShaderObject*& head);
		void privRemove(ShaderObject* node, ShaderObject*& head);

	private:  // add

		ShaderObject* active;

		ShaderObject* pBone;
		ShaderObject* pNull;
	};

}

#endif

// ---  End of File ---
