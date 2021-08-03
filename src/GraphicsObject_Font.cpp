
#include "GraphicsObject_Font.h"
#include "TextureMan.h"
#include "Model.h"
#include "Camera.h"
#include "CameraMan.h"
#include "GlyphMan.h"

namespace Azul
{
	GraphicsObject_Font::GraphicsObject_Font(Model* model, ShaderObject* pShaderObj, Font* _pFont)
		: GraphicsObject(model, pShaderObj), pFont(_pFont), offsetIdx(0), offsetX(0)
	{
		// Create the uv/orig matrix now, but set their values just in time as we render each letter
		this->poMatrix_uv = new Matrix();
		this->poMatrix_orig = new Matrix();
	}

	GraphicsObject_Font::~GraphicsObject_Font()
	{
		delete this->poMatrix_uv;
		delete this->poMatrix_orig;
	}

	void GraphicsObject_Font::SetState()
	{
		glActiveTexture(GL_TEXTURE0);

		// Bind the texture
		char c = this->pFont->pMessage[offsetIdx];
		Glyph* pGlyph = GlyphMan::Find(this->pFont->glyphName, (int)c);
		GLuint textureID = pGlyph->GetTexture()->textureID;
		glBindTexture(GL_TEXTURE_2D, textureID);

		// comment out one
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glFrontFace(GL_CW);

		// Blends for sprites
		glEnable(GL_BLEND);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);

		offsetIdx++;
	}

	void GraphicsObject_Font::SetDataGPU()
	{
		// Get camera
		Camera* pCam = CameraMan::GetCurrent(Camera::Type::ORTHOGRAPHIC_2D);

		// set the vao
		glBindVertexArray(this->GetModel()->vao);

		// use this shader
		this->pShaderObj->SetActive();

		// World matrix ----------------------------
		Matrix TransToOriginLowerLeft(Matrix::Trans::XYZ, (float)-pCam->getScreenWidth() / 2.0f, (float)-pCam->getScreenHeight() / 2.0f, 0.0f);
		Matrix tmpMatrix = *this->poMatrix_orig * this->GetWorld() * TransToOriginLowerLeft;

		glUniformMatrix4fv(this->pShaderObj->GetLocation("proj_matrix"), 1, GL_FALSE, (float*)&pCam->getProjMatrix());
		glUniformMatrix4fv(this->pShaderObj->GetLocation("view_matrix"), 1, GL_FALSE, (float*)&pCam->getViewMatrix());
		glUniformMatrix4fv(this->pShaderObj->GetLocation("world_matrix"), 1, GL_FALSE, (float*)&tmpMatrix);
		glUniformMatrix4fv(this->pShaderObj->GetLocation("uv_matrix"), 1, GL_FALSE, (float*)this->poMatrix_uv);


		offsetX += this->origWidth;
	}

	void GraphicsObject_Font::Draw()
	{
		// draw
		glDrawElements(GL_TRIANGLES, 3 * this->GetModel()->numTris, GL_UNSIGNED_INT, 0);   //The starting point of the IBO
	}

	void GraphicsObject_Font::RestoreState()
	{
		glDisable(GL_BLEND);
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);
	}
	bool GraphicsObject_Font::SetLetter(float startX, float startY)
	{
		char* pMessage = pFont->pMessage;
		char c = *(pMessage + offsetIdx);

		if (c == 0)
		{
			offsetIdx = 0;
			offsetX = 0;
			return false;
		}

		int key = (int)c;

		Glyph* pGlyph = GlyphMan::Find(pFont->glyphName, key);
		assert(pGlyph != nullptr);

		Rect subRect = pGlyph->GetSubRect();

		float w = subRect.width / (float)pGlyph->GetTexture()->width;
		float h = subRect.height / (float)pGlyph->GetTexture()->height;
		float u = subRect.x / (float)pGlyph->GetTexture()->width;
		float v = subRect.y / (float)pGlyph->GetTexture()->height;
		Matrix ScaleUV(Matrix::Scale::XYZ, w, h, 1.0f);
		Matrix TransUV(Matrix::Trans::XYZ, u, v, 0.0f);
		*this->poMatrix_uv = ScaleUV * TransUV;


		this->origWidth = subRect.width;
		this->origHeight = subRect.height;

		this->origPosX = startX + offsetX;
		this->origPosY = startY;

		Matrix ScaleXY(Matrix::Scale::XYZ, this->origWidth, this->origHeight, 1.0f);
		Matrix TransXY(Matrix::Trans::XYZ, this->origPosX, this->origPosY, 1.0f);
		*this->poMatrix_orig = ScaleXY *TransXY;
	
		return true;
	}
}

// --- End of File ---
