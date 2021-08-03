#include "GraphicsObject_SkinColorLight.h"
#include "CameraMan.h"

Azul::GraphicsObject_SkinColorLight::GraphicsObject_SkinColorLight(const Model* const _pModel, const ShaderObject* const _pShaderObj, Vect& objColor, Vect& LightColor, Vect& LightPos)
	:GraphicsObject(_pModel, _pShaderObj)
{
	this->poObjColor = new Vect(objColor);
	this->poLightColor = new Vect(LightColor);
	this->poLightPos = new Vect(LightPos);
	assert(poLightColor);
	assert(poLightPos);
	assert(this->pModel);
	assert(this->pShaderObj);
	assert(this->poWorld);
}

Azul::GraphicsObject_SkinColorLight::~GraphicsObject_SkinColorLight()
{
	delete poObjColor;
	delete poLightColor;
	delete poLightPos;
}

void Azul::GraphicsObject_SkinColorLight::SetState()
{
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
}

void Azul::GraphicsObject_SkinColorLight::SetDataGPU()
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

	Vect lightPos = *this->poLightPos;
	Vect lightColor = *this->poLightColor;
	Vect objColor = *this->poObjColor;
	glUniform3fv(this->pShaderObj->GetLocation("vLightPos"), 1, (float*)&lightPos);
	glUniform4fv(this->pShaderObj->GetLocation("vLightColor"), 1, (float*)&lightColor);
	glUniform4fv(this->pShaderObj->GetLocation("vObjColor"), 1, (float*)&objColor);
}

void Azul::GraphicsObject_SkinColorLight::Draw()
{
	//The starting point of the IBO
	glDrawElements(GL_TRIANGLES, 3 * this->GetModel()->numTris, GL_UNSIGNED_INT, 0);
}

void Azul::GraphicsObject_SkinColorLight::RestoreState()
{
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
}
