#ifndef GRAPHICS_OBJECT_SKIN_TEXTURE_LIGHT_H
#define GRAPHICS_OBJECT_SKIN_TEXTURE_LIGHT_H

#include "GraphicsObject.h"
#include "Texture.h"
#include "MathEngine.h"

namespace Azul {

	class GraphicsObject_SkinTextureLight : public GraphicsObject
	{
	public:
		GraphicsObject_SkinTextureLight(const Model* const pModel, const ShaderObject* const pShaderObj, Texture::Name _name, Vect& LightColor, Vect& LightPos);

		GraphicsObject_SkinTextureLight() = delete;
		GraphicsObject_SkinTextureLight(const GraphicsObject_SkinTextureLight&) = delete;
		GraphicsObject_SkinTextureLight& operator = (const GraphicsObject_SkinTextureLight&) = delete;
		~GraphicsObject_SkinTextureLight();

		// Rendermaterial contract
		virtual void SetState() override;
		virtual void SetDataGPU() override;
		virtual void Draw() override;
		virtual void RestoreState() override;

		// data:  place uniform instancing here
		Texture::Name textName;
		Vect* poLightColor;
		Vect* poLightPos;
	};
}

#endif