#include "FontMan.h"

namespace Azul {
	FontMan::FontMan()
	{
		this->active = nullptr;
	}

	void FontMan::Add(Font::Name name, const char* const pMessage, Glyph::Name glyphName)
	{
		FontMan* pFontMan = FontMan::privGetInstance();
		assert(pFontMan != nullptr);

		Font* pNode = new Font(name, pMessage, glyphName);
		pFontMan->privAddToFront(pNode, pFontMan->active);
	}

	Font* FontMan::Find(Font::Name _name)
	{
		// Get the instance to the manager
		FontMan* pFontMan = FontMan::privGetInstance();

		Font* pNode = (Font*)pFontMan->active;
		while (pNode != 0)
		{
			if (pNode->name == _name)
			{
				// found it
				break;
			}

			pNode = (Font*)pNode->next;
		}

		return pNode;
	}
	void FontMan::Destroy()
	{
		FontMan* pFontMan = FontMan::privGetInstance();
		assert(pFontMan);

		FontLink* pLink = pFontMan->active;

		while (pLink != nullptr)
		{
			FontLink* pTmp = pLink;
			pLink = pLink->next;
			pFontMan->privRemove(pTmp, pFontMan->active);
			delete pTmp;
		}
	}

	void FontMan::Create()
	{
		FontMan* pFontMan = FontMan::privGetInstance();
		assert(pFontMan);
	}

	void FontMan::privAddToFront(FontLink* node, FontLink*& head)
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

	void FontMan::privRemove(FontLink* pNode, FontLink*& poHead)
	{
		assert(pNode);

		if (pNode->prev != nullptr)
		{	// middle or last node
			pNode->prev->next = pNode->next;
		}
		else
		{  // first
			poHead = pNode->next;
		}

		if (pNode->next != nullptr)
		{	// middle node
			pNode->next->prev = pNode->prev;
		}
	}

	FontMan* FontMan::privGetInstance()
	{
		// This is where its actually stored (BSS section)
		static FontMan fontMan;
		return &fontMan;
	}

}