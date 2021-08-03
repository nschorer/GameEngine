
#ifndef GRAPHICS_OBJECT_FONT_H
#define GRAPHICS_OBJECT_FONT_H

#include "Texture.h"
#include "Font.h"
#include "GraphicsObject.h"
#include "ShaderObject.h"

namespace Azul
{
	class GraphicsObject_Font : public GraphicsObject
	{
	public:
		GraphicsObject_Font(Model* pModel, ShaderObject* pShaderObj, Font* font);
		virtual ~GraphicsObject_Font();

		// Rendermaterial contract
		virtual void SetState() override;
		virtual void SetDataGPU() override;
		virtual void Draw() override;
		virtual void RestoreState() override;

		bool SetLetter(float startX, float startY);

		// data:  place uniform instancing here

			//GLuint textureID;
		Font* pFont;
		Matrix* poMatrix_uv;
		Matrix* poMatrix_orig;

		float origWidth;
		float origHeight;

		float origPosX;
		float origPosY;
		unsigned int offsetIdx;
		float offsetX;
	};
}

#endif

//--- End of File ---

