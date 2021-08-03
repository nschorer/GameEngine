//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GRAPHICS_OBJECT_H
#define GRAPHICS_OBJECT_H

#include "MathEngine.h"
#include "ShaderObject.h"
#include "RenderMaterial.h"
#include "Model.h"
#include "CustomModel.h"
#include "BoundingSphere.h"

namespace Azul
{
	class GraphicsObject : public RenderMaterial
	{
	public:
		GraphicsObject(const Model* const model, const ShaderObject* const pShaderObj);

		GraphicsObject() = delete;
		GraphicsObject(const GraphicsObject&) = delete;
		GraphicsObject& operator = (const GraphicsObject&) = delete;
		virtual ~GraphicsObject();

		void Render();
		void SetWorld(Azul::Matrix& _world);

		const Model* GetModel() const;
		Matrix& GetWorld();

		Sphere* GetBoundingSphereForCollision();
		Sphere* poCurrSphere; // make this private

	private:
		void privUpdateBoundingSphereForCollision();

	protected:
		const Model* pModel;
		Matrix* poWorld;
		const ShaderObject* pShaderObj;
	};

}

#endif

// ---  End of File ---
