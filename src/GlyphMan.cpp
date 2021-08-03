#include "GlyphMan.h"
#include "pugixml.hpp"

namespace Azul {
	GlyphMan::GlyphMan()
	{
		this->active = nullptr;
	}

	GlyphMan* GlyphMan::privGetInstance()
	{
		// This is where its actually stored (BSS section)
		static GlyphMan glyphMan;
		return &glyphMan;
	}
	void GlyphMan::privAddToFront(GlyphLink* node, GlyphLink*& head)
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
	void GlyphMan::privRemove(GlyphLink* pNode, GlyphLink*& poHead)
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
	void GlyphMan::AddXML(Glyph::Name glyphName, const char* const assetName, Texture::Name textName)
	{
		int key = -1;
		int x = -1;
		int y = -1;
		int width = -1;
		int height = -1;

		// *****
		// Read in the XML file
		// *****

		pugi::xml_document doc;
		assert(doc.load_file(assetName));

		pugi::xml_node characters = doc.child("fontMetrics");

		for (auto it=characters.begin(); it != characters.end(); ++it)
		{
			pugi::xml_node node = *it;	

			const char* c_key = node.attribute("key").value();
			sscanf_s(c_key, "%d", &key);



			const char* c_x = node.child_value("x");
			sscanf_s(c_x, "%d", &x);

			const char* c_y = node.child_value("y");
			sscanf_s(c_y, "%d", &y);

			const char* c_width = node.child_value("width");
			sscanf_s(c_width, "%d", &width);

			const char* c_height = node.child_value("height");
			sscanf_s(c_height, "%d", &height);

			// have all the data... so now create a glyph
			GlyphMan::Add(glyphName, key, textName, (float)x, (float)y, (float)width, (float)height);
		}
	}

	void GlyphMan::Add(Glyph::Name name, int key, Texture::Name textName, float x, float y, float width, float height)
	{
		GlyphMan* pMan = GlyphMan::privGetInstance();
		Glyph* pNode = new Glyph(name, key, textName, x, y, width, height);
		pMan->privAddToFront(pNode, pMan->active);
	}
	Glyph* GlyphMan::Find(Glyph::Name _name, int key)
	{
		// Get the instance to the manager
		GlyphMan* pGlyphMan = GlyphMan::privGetInstance();

		Glyph* pNode = (Glyph*)pGlyphMan->active;
		while (pNode != 0)
		{
			if (pNode->GetKey() == key && pNode->GetName() == _name)
			{
				// found it
				break;
			}

			pNode = (Glyph*)pNode->next;
		}

		return pNode;
	}
	void GlyphMan::Destroy()
	{
		GlyphMan* pGlyphMan = GlyphMan::privGetInstance();
		assert(pGlyphMan);

		GlyphLink* pLink = pGlyphMan->active;

		while (pLink != nullptr)
		{
			GlyphLink* pTmp = pLink;
			pLink = pLink->next;
			pGlyphMan->privRemove(pTmp, pGlyphMan->active);
			delete pTmp;
		}
	}
	void GlyphMan::Create()
	{
		GlyphMan* pGlyphMan = GlyphMan::privGetInstance();
		assert(pGlyphMan);
	}
}