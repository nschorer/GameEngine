#ifndef GRAPHICS_OBJECT_SKIN_COLOR_LIGHT_H
#define GRAPHICS_OBJECT_SKIN_COLOR_LIGHT_H

#include "GraphicsObject.h"

namespace Azul {
	class GraphicsObject_SkinColorLight : public GraphicsObject
	{
	public:
		GraphicsObject_SkinColorLight(const Model* const pModel, const ShaderObject* const pShaderObj, Vect& objColor, Vect& LightColor, Vect& LightPos);

		GraphicsObject_SkinColorLight() = delete;
		GraphicsObject_SkinColorLight(const GraphicsObject_SkinColorLight&) = delete;
		GraphicsObject_SkinColorLight& operator = (const GraphicsObject_SkinColorLight&) = delete;
		~GraphicsObject_SkinColorLight();

		// Rendermaterial contract
		virtual void SetState() override;
		virtual void SetDataGPU() override;
		virtual void Draw() override;
		virtual void RestoreState() override;

		// data:  place uniform instancing here
		Vect* poObjColor;
		Vect* poLightColor;
		Vect* poLightPos;
	};
}

#endif