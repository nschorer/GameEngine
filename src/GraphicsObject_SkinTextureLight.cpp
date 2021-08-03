#include "GraphicsObject_SkinTextureLight.h"
#include "Model.h"
#include "TextureMan.h"
#include "CameraMan.h"
#include "ShaderObject.h"

namespace Azul {
	GraphicsObject_SkinTextureLight::GraphicsObject_SkinTextureLight(const Model* const _pModel, const ShaderObject* const _pShaderObj, Texture::Name _name, Vect& LightColor, Vect& LightPos)
		: GraphicsObject(_pModel, _pShaderObj), textName(_name)
	{
		this->poLightColor = new Vect(LightColor);
		this->poLightPos = new Vect(LightPos);
		assert(poLightColor);
		assert(poLightPos);
		assert(this->pModel);
		assert(this->pShaderObj);
		assert(this->poWorld);
	}

	GraphicsObject_SkinTextureLight::~GraphicsObject_SkinTextureLight()
	{
		delete poLightColor;
		delete poLightPos;
	}

	void GraphicsObject_SkinTextureLight::SetState()
	{
		glActiveTexture(GL_TEXTURE0);

		// Bind the texture
		GLuint textureID = TextureMan::FindID(this->textName);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
	}

	void GraphicsObject_SkinTextureLight::SetDataGPU()
	{
		// use this shader
		this->pShaderObj->SetActive();

		// set the vao
		glBindVertexArray(this->GetModel()->vao);

		Camera* pCam = CameraMan::GetCurrent(Camera::Type::PERSPECTIVE_3D);
		assert(pCam);

		Matrix view = pCam->getViewMatrix();
		Matrix proj = pCam->getProjMatrix();

		glUniformMatrix4fv(this->pShaderObj->GetLocation("proj_matrix"), 1, GL_FALSE, (float*)&proj);
		glUniformMatrix4fv(this->pShaderObj->GetLocation("view_matrix"), 1, GL_FALSE, (float*)&view);

		glUniformMatrix4fv(this->pShaderObj->GetLocation("invPose"), (int)pModel->numBones, GL_FALSE, (float*)pModel->poInversePose);
		glUniformMatrix4fv(this->pShaderObj->GetLocation("boneWorld"), (int)pModel->numBones, GL_FALSE, (float*)pModel->poBone);

		Vect light = *this->poLightPos;
		Vect color = *this->poLightColor;
		glUniform3fv(this->pShaderObj->GetLocation("vLightPos"), 1, (float*)&light);
		glUniform4fv(this->pShaderObj->GetLocation("vColor"), 1, (float*)&color);
	
		// Texture - linked to Texture 0
		glUniform1i(this->pShaderObj->GetLocation("tex_object"), 0);
	}

	void GraphicsObject_SkinTextureLight::Draw()
	{
		//The starting point of the IBO
		glDrawElements(GL_TRIANGLES, 3 * this->GetModel()->numTris, GL_UNSIGNED_INT, 0);
	}

	void GraphicsObject_SkinTextureLight::RestoreState()
	{
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
	}

}
