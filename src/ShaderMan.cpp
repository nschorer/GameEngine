//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "ShaderMan.h"

namespace Azul
{
	ShaderMan::ShaderMan()
	{
		this->active = nullptr;
	}

	ShaderMan* ShaderMan::privGetInstance()
	{
		// This is where its actually stored (BSS section)
		static ShaderMan shaderMan;
		return &shaderMan;
	}


	void ShaderMan::Create()
	{
		ShaderMan* pShaderMan;
		pShaderMan = ShaderMan::privGetInstance();
		assert(pShaderMan);
	}

	void ShaderMan::Destroy()
	{
		ShaderMan* pShaderMan = ShaderMan::privGetInstance();
		assert(pShaderMan);

		ShaderObject* pLink = pShaderMan->active;

		while (pLink != nullptr)
		{
			ShaderObject* pTmp = pLink;
			pLink = (ShaderObject*)pLink->next;
			pShaderMan->privRemove(pTmp, pShaderMan->active);
			delete pTmp;
		}
	}

	ShaderObject* ShaderMan::Find(ShaderObject::Name _name)
	{
		ShaderMan* pShaderMan = ShaderMan::privGetInstance();
		assert(pShaderMan);

		ShaderObject* pLink = pShaderMan->active;

		while (pLink != nullptr)
		{
			if (pLink->name == _name)
				break;

			pLink = (ShaderObject*)pLink->next;
		}

		return pLink;
	}

	void ShaderMan::SetBone(ShaderObject* pShader)
	{
		ShaderMan* pShaderMan = ShaderMan::privGetInstance();
		assert(pShaderMan);

		pShaderMan->pBone = pShader;
	}

	ShaderObject* ShaderMan::GetBone()
	{
		ShaderMan* pShaderMan = ShaderMan::privGetInstance();
		assert(pShaderMan);

		return pShaderMan->pBone;
	}

	void ShaderMan::SetNull(ShaderObject* pShader)
	{
		ShaderMan* pShaderMan = ShaderMan::privGetInstance();
		assert(pShaderMan);

		pShaderMan->pNull = pShader;
	}

	ShaderObject* ShaderMan::GetNull()
	{
		ShaderMan* pShaderMan = ShaderMan::privGetInstance();
		assert(pShaderMan);

		return pShaderMan->pNull;
	}

	void ShaderMan::Add(ShaderObject* pShader)
	{
		// Get the instance to the manager
		ShaderMan* pShaderMan = ShaderMan::privGetInstance();
		assert(pShaderMan);

		// Create a TextureNode
		assert(pShader);

		// Now add it to the manager
		pShaderMan->privAddToFront(pShader, pShaderMan->active);
	}

	void ShaderMan::privAddToFront(ShaderObject* node, ShaderObject*& head)
	{
		assert(node != 0);

		if (head == 0)
		{
			head = node;
			node->next = 0;
			node->prev = 0;
		}
		else
		{
			node->next = head;
			head->prev = node;
			head = node;
		}
	}

	void ShaderMan::privRemove(ShaderObject* pNode, ShaderObject*& poHead)
	{
		assert(pNode);

		if (pNode->prev != nullptr)
		{	// middle or last node
			pNode->prev->next = pNode->next;
		}
		else
		{  // first
			poHead = (ShaderObject*)pNode->next;
		}

		if (pNode->next != nullptr)
		{	// middle node
			pNode->next->prev = pNode->prev;
		}

	}

}

// ---  End of File ---
