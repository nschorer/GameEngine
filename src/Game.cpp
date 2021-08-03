//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "sb7.h"
#include "Game.h"
#include "MathEngine.h"
#include "GameObject.h"
#include "SimpleGameObject.h"
#include "GameObjectRigid.h"
#include "GameObjectAnim.h"
#include "GameObject2D.h"
#include "ShaderObject.h"
#include "CubeModel.h"
#include "PyramidModel.h"
#include "CrossModel.h"
#include "SphereModel.h"
#include "SpriteModel.h"
#include "CustomModel.h"
#include "TeddyModel.h"
#include "GraphicsObject.h"
#include "GraphicsObject_ColorByPosition.h"
#include "GraphicsObject_Wireframe.h"
#include "GraphicsObject_FlatTexture.h"
#include "GraphicsObject_TextureLight.h"
#include "GraphicsObject_ConstColor.h"
#include "GraphicsObject_WireframeConstantColor.h"
#include "GraphicsObject_SkinTexture.h"
#include "GraphicsObject_SkinTextureLight.h"
#include "GraphicsObject_SkinColorLight.h"
#include "TextureMan.h"
#include "InputMan.h"
#include "InputTest.h"
#include "Camera.h"
#include "CameraMan.h"
#include "GameObjectMan.h"
#include "ModelMan.h"
#include "ShaderMan.h"
#include "BoundingSphereGameObject.h"
#include "AnimTime.h"
#include "AnimTimer.h"
#include "Anim.h"
#include "TimerController.h"
#include "AnimController.h"
#include "AnimControllerMan.h"
#include "Clip.h"
#include"ClipMan.h"

#include "ImageMan.h"
#include "GraphicsObject_Sprite.h"
#include "GlyphMan.h"
#include "FontMan.h"
#include "GraphicsObject_Font.h"
#include "GameObjectFont.h"

#include "Mixer.h"
#include "AnimTransition.h"

namespace Azul
{

	// TO DO: ----------------------------------
	GraphicsObject_TextureLight* pGObj;
	bool camDestroyed = false;
	GameObject* pGlobalGO = nullptr;

	Clip::Name humanClip;
	Clip::Name humanNCClip;
	Clip::Name teddyClip;

	float cameraSpeed = 10.0f;

	// Demo
	Vect Orig_up;
	Vect Orig_tar;
	Vect Orig_pos;
	Vect Orig_upNorm;
	Vect Orig_forwardNorm;
	Vect Orig_RightNorm;

	//-----------------------------------------------------------------------------
	//  Game::Game()
	//		Game Engine Constructor
	//-----------------------------------------------------------------------------
	Game::Game(const char* const pWindowName, const int Width, const int Height)
		:Engine(pWindowName, Width, Height)
	{
		assert(pWindowName);
		this->globalTimer.Tic();
		this->intervalTimer.Tic();
	}

	//-----------------------------------------------------------------------------
	// Game::Initialize()
	//		Allows the engine to perform any initialization it needs to before 
	//      starting to run.  This is where it can query for any required services 
	//      and load any non-graphic related content. 
	//-----------------------------------------------------------------------------
	void Game::Initialize()
	{

	}


	//-----------------------------------------------------------------------------
	// Game::LoadContent()
	//		Allows you to load all content needed for your engine,
	//	    such as objects, graphics, etc.
	//-----------------------------------------------------------------------------
	void Game::LoadContent()
	{
		//*******************************************************************
		// Setup Managers
		//*******************************************************************
		InputMan::Create();
		ModelMan::Create();
		ShaderMan::Create();
		CameraMan::Create();
		GameObjectMan::Create();
		AnimControllerMan::Create();
		ClipMan::Create();
		ImageMan::Create();
		GlyphMan::Create();
		FontMan::Create();

		//*******************************************************************
		// Setup Other Singletons
		//*******************************************************************
		Mixer::Create();

		//*******************************************************************
		// Setup Cameras
		//*******************************************************************
		Camera* pCam = new Camera(Camera::Name::MAIN, Camera::Type::PERSPECTIVE_3D);
		assert(pCam);
		pCam->setViewport(0, 0, this->info.windowWidth, this->info.windowHeight);
		pCam->setPerspective(50.0f, float(pCam->getScreenWidth()) / float(pCam->getScreenHeight()), 0.10f, 5000.0f);
		//pCam->setOrientAndPosition(Vect(0.0f, 1.0f, 0.0f), Vect(0.0f, 0.0f, 0.0f), Vect(0.0f, 0.0f, -1000));
		pCam->setOrientAndPosition(Vect(0.0f, 0.0f, 1.0f), Vect(0.0f, 0.0f, 500.0f), Vect(0.0f, -3500.0f, 750.0f));
		pCam->updateCamera();
		CameraMan::Add(pCam);
		CameraMan::SetCurrent(Camera::Name::MAIN, Camera::Type::PERSPECTIVE_3D);


		pCam = new Camera(Camera::Name::ORTHO, Camera::Type::ORTHOGRAPHIC_2D);
		assert(pCam);
		pCam->setViewport(0, 0, this->info.windowWidth, this->info.windowHeight);
		pCam->setOrthographic((float)-pCam->getScreenWidth() / 2.0f, (float)pCam->getScreenWidth() / 2.0f,
			(float)-pCam->getScreenHeight() / 2.0f, (float)pCam->getScreenHeight() / 2.0f,
			1.0f, 1000.0f);
		pCam->setOrientAndPosition(Vect(0.0f, 1.0f, 0.0f), Vect(0.0f, 0.0f, -1.0f), Vect(0.0f, 0.0f, 2.0f));
		CameraMan::Add(pCam);
		CameraMan::SetCurrent(Camera::Name::ORTHO, Camera::Type::ORTHOGRAPHIC_2D);

		pCam = CameraMan::Find(Camera::Name::MAIN);
		pCam->setViewport(0, 0, this->info.windowWidth, this->info.windowHeight);
		assert(pCam);


		//*******************************************************************
		// Setup Textures -- do this before models in case models need these textures
		//*******************************************************************
		TextureMan::Add("HotPink.tga", Texture::Name::DEFAULT);
		TextureMan::Add("Rocks.tga", Texture::Name::ROCKS);
		TextureMan::Add("Stone.tga", Texture::Name::STONES);
		TextureMan::Add("RedBrick.tga", Texture::Name::RED_BRICK);
		TextureMan::Add("Duckweed.tga", Texture::Name::DUCKWEED);
		TextureMan::Add("Aliens.tga", Texture::Name::INVADERS);
		//TextureMan::Add("TeddyTexNORLE.tga", Texture::Name::TEDDY);

		TextureMan::Add("Consolas36pt.tga", Texture::Name::CONSOLAS36PT);
		GlyphMan::AddXML(Glyph::Name::Consoloas36pt, "Consolas36pt.xml", Texture::Name::CONSOLAS36PT);

		//*******************************************************************
		// Setup Images
		//*******************************************************************
		Image* pImageGreen = new Image(Image::Name::Alien_Green, Texture::Name::INVADERS, Rect(136.0f, 64.0f, 85.0f, 63.0f));

		assert(pImageGreen);
		ImageMan::Add(pImageGreen);

		//*******************************************************************
		// Setup Models
		//*******************************************************************
		CubeModel* pCubeModel = new CubeModel("cubeModel.azul");
		assert(pCubeModel);
		ModelMan::Add(pCubeModel);

		PyramidModel* pPyramidModel = new PyramidModel("pyramidModel_ccw.azul");
		assert(pPyramidModel);
		ModelMan::Add(pPyramidModel);
		ModelMan::SetBone(pPyramidModel);

		CrossModel* pCrossModel = new CrossModel("crossModel.azul");
		assert(pCrossModel);
		ModelMan::Add(pCrossModel);

		SphereModel* pSphereModel = new SphereModel("sphereModel_ccw.azul");
		assert(pSphereModel);
		ModelMan::Add(pSphereModel);

		SpriteModel* pSpriteModel = new SpriteModel("SpriteModel.azul");
		assert(pSpriteModel);
		ModelMan::Add(pSpriteModel);

		CustomModel* pShip = new CustomModel("space_frigate.azul");
		assert(pShip);
		ModelMan::Add(pShip);

		CustomModel* pBoy = new CustomModel("astroboy.azul");
		assert(pBoy);
		ModelMan::Add(pBoy);

		CustomModel* pBuggy = new CustomModel("buggy.azul");
		assert(pBuggy);
		ModelMan::Add(pBuggy);

		CustomModel* pBear = new CustomModel("warbear_normal.azul");
		assert(pBear);
		ModelMan::Add(pBear);

		CustomModel* pPolar = new CustomModel("warbear_polar.azul");
		assert(pPolar);
		ModelMan::Add(pPolar);

		CustomModel* pRobo = new CustomModel("robot.azul");
		assert(pRobo);
		ModelMan::Add(pRobo);

		TeddyModel* pTeddy = new TeddyModel("TeddyModel.azul");
		assert(pTeddy);
		ModelMan::Add(pTeddy);

		CustomModel* pCustomTeddy = new CustomModel("TeddyAnimModel.azul");
		assert(pCustomTeddy);
		ModelMan::Add(pCustomTeddy);

		CustomModel* pCustomHumanoid = new CustomModel("HumanoidAnimModel.azul");
		assert(pCustomHumanoid);
		ModelMan::Add(pCustomHumanoid);

		CustomModel* pCustomHumanoid2 = new CustomModel("Humanoid2AnimModel.azul");
		assert(pCustomHumanoid2);
		ModelMan::Add(pCustomHumanoid2);

		//*******************************************************************
		// Setup Shaders
		//*******************************************************************
		ShaderObject* pShaderObject_color = new ShaderObject(ShaderObject::Name::COLOR_LIGHT, "colorRender", ShaderObject::Type::GRAPHICS);
		assert(pShaderObject_color);
		ShaderMan::Add(pShaderObject_color);

		ShaderObject* pShaderObject_texture = new ShaderObject(ShaderObject::Name::TEXTURE_SIMPLE, "textureRender", ShaderObject::Type::GRAPHICS);
		assert(pShaderObject_texture);
		ShaderMan::Add(pShaderObject_texture);

		ShaderObject* pShaderObject_textureLight = new ShaderObject(ShaderObject::Name::TEXTURE_POINT_LIGHT, "texturePointLightDiff", ShaderObject::Type::GRAPHICS);
		assert(pShaderObject_textureLight);
		ShaderMan::Add(pShaderObject_textureLight);
		ShaderMan::SetBone(pShaderObject_textureLight);

		ShaderObject* pShaderObject_constColor = new ShaderObject(ShaderObject::Name::CONST_COLOR, "constcolorRender", ShaderObject::Type::GRAPHICS);
		assert(pShaderObject_constColor);
		ShaderMan::Add(pShaderObject_constColor);

		ShaderObject* pShaderObject_colorSingle = new ShaderObject(ShaderObject::Name::COLOR_SINGLE, "colorSingleRender", ShaderObject::Type::GRAPHICS);
		assert(pShaderObject_colorSingle);
		ShaderMan::Add(pShaderObject_colorSingle);

		ShaderObject* pShaderObject_null = new ShaderObject(ShaderObject::Name::NULL_SHADER, "nullRender", ShaderObject::Type::GRAPHICS);
		assert(pShaderObject_null);
		ShaderMan::Add(pShaderObject_null);
		ShaderMan::SetNull(pShaderObject_null);

		ShaderObject* pShaderObject_sprite = new ShaderObject(ShaderObject::Name::SPRITE, "spriteRender", ShaderObject::Type::GRAPHICS);
		assert(pShaderObject_sprite);
		ShaderMan::Add(pShaderObject_sprite);

		ShaderObject* pShaderObject_skinTexture = new ShaderObject(ShaderObject::Name::SKIN_TEXTURE, "skinTextureRender", ShaderObject::Type::GRAPHICS);
		assert(pShaderObject_skinTexture);
		ShaderMan::Add(pShaderObject_skinTexture);

		ShaderObject* pShaderObject_skinTextureLight = new ShaderObject(ShaderObject::Name::SKIN_TEXTURE_LIGHT, "skinTextureLightRender", ShaderObject::Type::GRAPHICS);
		assert(pShaderObject_skinTextureLight);
		ShaderMan::Add(pShaderObject_skinTextureLight);

		ShaderObject* pShaderObject_skinColorLight = new ShaderObject(ShaderObject::Name::SKIN_COLOR_LIGHT, "skinColorLightRender", ShaderObject::Type::GRAPHICS);
		assert(pShaderObject_skinColorLight);
		ShaderMan::Add(pShaderObject_skinColorLight);

		ShaderObject* pShaderObject_computeMixer = new ShaderObject(ShaderObject::Name::COMPUTE_MIXER, "computeMixer", ShaderObject::Type::COMPUTE);
		assert(pShaderObject_computeMixer);
		ShaderMan::Add(pShaderObject_computeMixer);

		ShaderObject* pShaderObject_computeLocalToWorld = new ShaderObject(ShaderObject::Name::COMPUTE_LOCAL_TO_WORLD, "computeWorld", ShaderObject::Type::COMPUTE);
		assert(pShaderObject_computeLocalToWorld);
		ShaderMan::Add(pShaderObject_computeLocalToWorld);

		//*******************************************************************
		// Fonts
		//*******************************************************************
		FontMan::Add(Font::Name::TestMessage1, "Ratio 1: 1000", Glyph::Name::Consoloas36pt);
		FontMan::Add(Font::Name::TestMessage2, "Compressed", Glyph::Name::Consoloas36pt);
		FontMan::Add(Font::Name::TestMessage3, "Original", Glyph::Name::Consoloas36pt);

		Font* pFont = FontMan::Find(Font::Name::TestMessage1);

		GraphicsObject_Font* pGraphics_Font = new GraphicsObject_Font(pSpriteModel, pShaderObject_sprite, pFont);
		GameObjectFont* pFontObj = new GameObjectFont(pGraphics_Font);
		GameObjectMan::Add(pFontObj, GameObjectMan::GetRoot());
		pFontObj->posX = 250.0f;
		pFontObj->posY = 50.0f;

		pFont = FontMan::Find(Font::Name::TestMessage2);

		pGraphics_Font = new GraphicsObject_Font(pSpriteModel, pShaderObject_sprite, pFont);
		pFontObj = new GameObjectFont(pGraphics_Font);
		GameObjectMan::Add(pFontObj, GameObjectMan::GetRoot());
		pFontObj->posX = 50.0f;
		pFontObj->posY = 400.0f;

		pFont = FontMan::Find(Font::Name::TestMessage3);

		pGraphics_Font = new GraphicsObject_Font(pSpriteModel, pShaderObject_sprite, pFont);
		pFontObj = new GameObjectFont(pGraphics_Font);
		GameObjectMan::Add(pFontObj, GameObjectMan::GetRoot());
		pFontObj->posX = 450.0f;
		pFontObj->posY = 400.0f;

		//*******************************************************************
		// Game Objects / Graphics Objects
		//*******************************************************************

		//2d objects should get created first... will make sure they are rendered last

		// Sprite
		/*GraphicsObject_Sprite* pGraphics_Sprite = new GraphicsObject_Sprite(pSpriteModel, pShaderObject_sprite, pImageGreen, Rect(455, 155, 150, 150));
		GameObject2D* p2DObj = new GameObject2D(pGraphics_Sprite);
		GameObjectMan::Add(p2DObj, GameObjectMan::GetRoot());
		p2DObj->posX = 900.0f;
		p2DObj->posY = 450.0f;

		p2DObj->SetName("ALIEN");*/


		//GameObjectRigid* pGameObj = nullptr;
		//GraphicsObject* pGraphicsObj = nullptr;
		Vect color(1.50f, 1.50f, 1.50f, 1.0f);
		Vect pos(1.0f, 1.0f, 1.0f);
		Vect Yellow(1.0f, 1.0f, 0.0f, 1.0f);

		GraphicsObject* pSkinGraphicsObject = new GraphicsObject_SkinTextureLight(pCustomTeddy, pShaderObject_skinTextureLight, Texture::Name::TEDDY, color, pos);
		GameObjectRigid* pSkinObject = new GameObjectRigid(pSkinGraphicsObject);
		GameObjectMan::Add(pSkinObject, GameObjectMan::GetRoot());
		pSkinObject->SetName("Skin Object");

		// Humanoid
		Vect Green(0.0f, 1.0f, 0.0f, 1.0f);

		pSkinGraphicsObject = new GraphicsObject_SkinColorLight(pCustomHumanoid, pShaderObject_skinColorLight, Green, color, pos);
		pSkinObject = new GameObjectRigid(pSkinGraphicsObject);
		GameObjectMan::Add(pSkinObject, GameObjectMan::GetRoot());
		pSkinObject->SetName("Skin Object");

		Vect Red(1.0f, 0.0f, 0.0f, 1.0f);

		pSkinGraphicsObject = new GraphicsObject_SkinColorLight(pCustomHumanoid2, pShaderObject_skinColorLight, Red, color, pos);
		pSkinObject = new GameObjectRigid(pSkinGraphicsObject);
		GameObjectMan::Add(pSkinObject, GameObjectMan::GetRoot());
		pSkinObject->SetName("Skin Object");


		//*******************************************************************
		// Animations
		//*******************************************************************

		
		//AnimControllerMan::Add(AnimController::Name::Teddy, "teddy.azul", pCustomTeddy);
		//teddyClip = Clip::Name::TEDDY_WALK;

		//AnimControllerMan::Add(AnimController::Name::Humanoid, "humanoid.azul", pCustomHumanoid);         
		//AnimControllerMan::Add(AnimController::Name::Test, "humanoid_NC.azul", pCustomHumanoid2);    

		//// Import the clips directly into the manager without creating a controller
		//ClipMan::ImportClips("humanoid_walk.azul");
		//ClipMan::ImportClips("humanoid_walk_NC.azul");

		//

		//AnimController* pAnim1 = AnimControllerMan::Find(AnimController::Name::Humanoid);
		//AnimController* pAnim2 = AnimControllerMan::Find(AnimController::Name::Test);
		//pAnim1->SetClip(ClipMan::Find(Clip::Name::HUMAN_RUN));
		//pAnim2->SetClip(ClipMan::Find(Clip::Name::HUMAN_RUN_NOT_COMPRESSED));

		// Load all the clips
		ClipMan::ImportClips2("teddy.azul", pCustomTeddy);
		ClipMan::ImportClips2("humanoid.azul", pCustomHumanoid);
		ClipMan::ImportClips2("humanoid_walk.azul", pCustomHumanoid);
		ClipMan::ImportClips2("humanoid_NC.azul", pCustomHumanoid2);
		ClipMan::ImportClips2("humanoid_walk_NC.azul", pCustomHumanoid2);

		// Create controllers
		AnimControllerMan::Add(AnimController::Name::Teddy, ClipMan::Find(Clip::Name::TEDDY_WALK));
		AnimControllerMan::Add(AnimController::Name::Humanoid, ClipMan::Find(Clip::Name::HUMAN_RUN));
		AnimControllerMan::Add(AnimController::Name::Test, ClipMan::Find(Clip::Name::HUMAN_RUN_NOT_COMPRESSED));



		humanClip = Clip::Name::HUMAN_RUN;
		humanNCClip = Clip::Name::HUMAN_RUN_NOT_COMPRESSED;

		float ratio = (float)ClipMan::Find(Clip::Name::HUMAN_RUN_NOT_COMPRESSED)->clipSize/ (float)ClipMan::Find(Clip::Name::HUMAN_RUN)->clipSize;

		std::string s = "1 : " + std::to_string(ratio);

		pFont = FontMan::Find(Font::Name::TestMessage1);
		pFont->UpdateMessage(s.c_str());

		//*******************************************************************
		// Demo controls
		//*******************************************************************

		// squirrel for reset
		pCam->updateCamera();
		pCam->GetHelper(Orig_up, Orig_tar, Orig_pos, Orig_upNorm, Orig_forwardNorm, Orig_RightNorm);

		//Trace::out("change texture(far left warbear): ty\n");
		Trace::out("Speed up/slow down animation: +-\n");
		Trace::out("Toggle animation: .,\n");
		Trace::out("Transition animation: ;\n");
		Trace::out("Toggle skeleton: x\n");

		
	}


	//-----------------------------------------------------------------------------
	// Game::Update()
	//      Called once per frame, update data, tranformations, etc
	//      Use this function to control process order
	//      Input, AI, Physics, Animation, and Graphics
	//-----------------------------------------------------------------------------

	int count = 0;
	void Game::Update(float currentTime)
	{
		AZUL_UNUSED_VAR(currentTime);

		ProcessInputs();

		/*Font* pFont = FontMan::Find(Font::Name::TestMessage1);
		std::string str = "dog " + std::to_string(count++);
		const char* cstr = str.c_str();
		pFont->UpdateMessage(cstr);*/

		// Needs to be updated once a frame
		CameraMan::GetCurrent(Camera::Type::PERSPECTIVE_3D)->updateCamera();
		Camera* pCam2D = CameraMan::GetCurrent(Camera::Type::ORTHOGRAPHIC_2D);
		pCam2D->updateCamera();

		// ------------------------------------------------
		// Animate ME
		// ------------------------------------------------
		AnimControllerMan::Update();

		// Update the game objects
		GameObjectMan::Update(this->globalTimer.Toc());
	}


	//-----------------------------------------------------------------------------
	// Game::Draw()
	//		This function is called once per frame
	//	    Use this for draw graphics to the screen.
	//      Only do rendering here
	//-----------------------------------------------------------------------------
	void Game::Draw()
	{
		GameObjectMan::Draw();
	}


	//-----------------------------------------------------------------------------
	// Game::UnLoadContent()
	//       unload content (resources loaded above)
	//       unload all content that was loaded before the Engine Loop started
	//-----------------------------------------------------------------------------
	void Game::UnLoadContent()
	{
		// Todo fix this - verify OpenGL removal

		//  glDeleteVertexArrays(1, &vao);
		//  glDeleteProgram(pShaderObject->programObject);
		//  glDeleteBuffers(1, &buffer);

		TextureMan::Destroy();
		InputMan::Destroy();
		GameObjectMan::Destroy();
		ClipMan::Destroy();
		AnimControllerMan::Destroy();
		ModelMan::Destroy();
		ShaderMan::Destroy();
		CameraMan::Destroy();
		ImageMan::Destroy();
		GlyphMan::Destroy();
		FontMan::Destroy();

		Mixer::Destroy();
	}

	//------------------------------------------------------------------
	// Game::ClearBufferFunc()
	// Allows user to change the way the clear buffer function works
	//------------------------------------------------------------------
	void Game::ClearBufferFunc()
	{
		const GLfloat grey[] = { 0.7f, 0.7f, 0.8f, 1.0f };
		const GLfloat one = 1.0f;

		glViewport(0, 0, info.windowWidth, info.windowHeight);
		glClearBufferfv(GL_COLOR, 0, grey);
		glClearBufferfv(GL_DEPTH, 0, &one);
	}

	void Game::ProcessInputs() {

		Keyboard* pKey = InputMan::GetKeyboard();

		if (pKey->GetKeyState(Keyboard::KEY_W))
		{

			Vect pos;
			Vect tar;
			Vect up;
			Vect upNorm;
			Vect forwardNorm;
			Vect rightNorm;

			CameraMan::GetCurrent(Camera::Type::PERSPECTIVE_3D)->GetHelper(up, tar, pos, upNorm, forwardNorm, rightNorm);

			// OK, now 3 points... pos, tar, up
			//     now 3 normals... upNorm, forwardNorm, rightNorm
			//	tar = obj;

			pos = pos + cameraSpeed * forwardNorm;
			tar = tar + cameraSpeed * forwardNorm;


			CameraMan::GetCurrent(Camera::Type::PERSPECTIVE_3D)->SetHelper(up, tar, pos);
		}

		if (pKey->GetKeyState(Keyboard::KEY_S))
		{

			Vect pos;
			Vect tar;
			Vect up;
			Vect upNorm;
			Vect forwardNorm;
			Vect rightNorm;

			CameraMan::GetCurrent(Camera::Type::PERSPECTIVE_3D)->GetHelper(up, tar, pos, upNorm, forwardNorm, rightNorm);

			// OK, now 3 points... pos, tar, up
			//     now 3 normals... upNorm, forwardNorm, rightNorm
			//tar = obj;

			pos = pos - cameraSpeed * forwardNorm;

			CameraMan::GetCurrent(Camera::Type::PERSPECTIVE_3D)->SetHelper(up, tar, pos);
		}

		if (pKey->GetKeyState(Keyboard::KEY_A))
		{

			Vect pos;
			Vect tar;
			Vect up;
			Vect upNorm;
			Vect forwardNorm;
			Vect rightNorm;

			CameraMan::GetCurrent(Camera::Type::PERSPECTIVE_3D)->GetHelper(up, tar, pos, upNorm, forwardNorm, rightNorm);

			// OK, now 3 points... pos, tar, up
			//     now 3 normals... upNorm, forwardNorm, rightNorm
			tar = tar - cameraSpeed * rightNorm;
			pos = pos - cameraSpeed * rightNorm;
			up = up -	cameraSpeed * rightNorm;

			CameraMan::GetCurrent(Camera::Type::PERSPECTIVE_3D)->SetHelper(up, tar, pos);
		}

		if (pKey->GetKeyState(Keyboard::KEY_D))
		{

			Vect pos;
			Vect tar;
			Vect up;
			Vect upNorm;
			Vect forwardNorm;
			Vect rightNorm;

			CameraMan::GetCurrent(Camera::Type::PERSPECTIVE_3D)->GetHelper(up, tar, pos, upNorm, forwardNorm, rightNorm);

			// OK, now 3 points... pos, tar, up
			//     now 3 normals... upNorm, forwardNorm, rightNorm
			tar = tar + cameraSpeed * rightNorm;
			pos = pos + cameraSpeed * rightNorm;
			up = up +	cameraSpeed * rightNorm;

			CameraMan::GetCurrent(Camera::Type::PERSPECTIVE_3D)->SetHelper(up, tar, pos);
		}

		if (pKey->GetKeyState(Keyboard::KEY_ARROW_RIGHT))
		{

			Vect pos;
			Vect tar;
			Vect up;
			Vect upNorm;
			Vect forwardNorm;
			Vect rightNorm;

			CameraMan::GetCurrent(Camera::Type::PERSPECTIVE_3D)->GetHelper(up, tar, pos, upNorm, forwardNorm, rightNorm);

			// OK, now 3 points... pos, tar, up
			//     now 3 normals... upNorm, forwardNorm, rightNorm
			//tar = obj;

			Matrix Rot(Matrix::Rot::AxisAngle, upNorm, -0.03f);

			Matrix Trans(Matrix::Trans::XYZ, pos);
			Matrix NegTrans(Matrix::Trans::XYZ, -pos);

			Matrix M = NegTrans * Rot * Trans;

			up = up * M;
			pos = pos * M;
			tar = tar * M;

			/*Matrix Trans(Matrix::Trans::XYZ, tar);
			Matrix NegTrans(Matrix::Trans::XYZ, -tar);

			Matrix Rot;
			Rot.set(Matrix::Rot::AxisAngle, upNorm, +0.03f);

			Matrix M = NegTrans * Rot * Trans;

			up = up * M;
			pos = pos * M;
			tar = tar * M;*/

			CameraMan::GetCurrent(Camera::Type::PERSPECTIVE_3D)->SetHelper(up, tar, pos);
		}

		if (pKey->GetKeyState(Keyboard::KEY_ARROW_LEFT))
		{

			Vect pos;
			Vect tar;
			Vect up;
			Vect upNorm;
			Vect forwardNorm;
			Vect rightNorm;

			CameraMan::GetCurrent(Camera::Type::PERSPECTIVE_3D)->GetHelper(up, tar, pos, upNorm, forwardNorm, rightNorm);

			// OK, now 3 points... pos, tar, up
			//     now 3 normals... upNorm, forwardNorm, rightNorm

			Matrix Rot(Matrix::Rot::AxisAngle, upNorm, +0.03f);

			Matrix Trans(Matrix::Trans::XYZ, pos);
			Matrix NegTrans(Matrix::Trans::XYZ, -pos);

			Matrix M = NegTrans * Rot * Trans;

			up = up * M;
			pos = pos * M;
			tar = tar * M;

			CameraMan::GetCurrent(Camera::Type::PERSPECTIVE_3D)->SetHelper(up, tar, pos);
		}

		if (pKey->GetKeyState(Keyboard::KEY_ARROW_DOWN))
		{
			Vect pos;
			Vect tar;
			Vect up;
			Vect upNorm;
			Vect forwardNorm;
			Vect rightNorm;

			CameraMan::GetCurrent(Camera::Type::PERSPECTIVE_3D)->GetHelper(up, tar, pos, upNorm, forwardNorm, rightNorm);

			//tar = obj;

			// OK, now 3 points... pos, tar, up
			//     now 3 normals... upNorm, forwardNorm, rightNorm

			Matrix Trans(Matrix::Trans::XYZ, pos);
			Matrix NegTrans(Matrix::Trans::XYZ, -pos);
			Matrix Rot;
			Rot.set(Matrix::Rot::AxisAngle, rightNorm, 0.03f);

			Matrix M = NegTrans * Rot * Trans;

			up = up * M;
			pos = pos * M;
			tar = tar * M;

			CameraMan::GetCurrent(Camera::Type::PERSPECTIVE_3D)->SetHelper(up, tar, pos);
		}

		if (pKey->GetKeyState(Keyboard::KEY_ARROW_UP))
		{
			Vect pos;
			Vect tar;
			Vect up;
			Vect upNorm;
			Vect forwardNorm;
			Vect rightNorm;

			CameraMan::GetCurrent(Camera::Type::PERSPECTIVE_3D)->GetHelper(up, tar, pos, upNorm, forwardNorm, rightNorm);

			// OK, now 3 points... pos, tar, up
			//     now 3 normals... upNorm, forwardNorm, rightNorm
			//tar = obj;

			Matrix Trans(Matrix::Trans::XYZ, pos);
			Matrix NegTrans(Matrix::Trans::XYZ, -pos);
			Matrix Rot;
			Rot.set(Matrix::Rot::AxisAngle, rightNorm, -0.03f);

			Matrix M = NegTrans * Rot * Trans;

			up = up * M;
			pos = pos * M;
			tar = tar * M;

			CameraMan::GetCurrent(Camera::Type::PERSPECTIVE_3D)->SetHelper(up, tar, pos);
		}

		static bool cPressed = false;
		if (pKey->GetKeyState(Keyboard::KEY_C))
		{
			if (!cPressed) {
				GameObjectMan::ToggleRenderCollision();
			}

			cPressed = true;
		}
		else {
			cPressed = false;
		}

		static bool xPressed = false;
		if (pKey->GetKeyState(Keyboard::KEY_X))
		{
			if (!xPressed) {
				GameObjectMan::ToggleRenderSkeleton();
			}

			xPressed = true;
		}
		else {
			xPressed = false;
		}


		//if (pKey->GetKeyState(Keyboard::KEY_R))
		//{
		//	GraphicsObject_FlatTexture* pGO = (GraphicsObject_FlatTexture*)pGlobalGO->GetGraphicsObject();
		//	CustomModel* pModel = (CustomModel*)pGO->GetModel();

		//	pGO->textName = pModel->GetTexture();
		//	pGO->SetState();
		//}

		//// Warbear costumes
		//if (pKey->GetKeyState(Keyboard::KEY_T))
		//{
		//	GraphicsObject_TextureLight* pGO = (GraphicsObject_TextureLight*)pGlobalGO->GetGraphicsObject();
		//	pGO->SetTexture(Texture::Name::WAR_BEAR_NORMAL);
		//}
		//if (pKey->GetKeyState(Keyboard::KEY_Y))
		//{
		//	GraphicsObject_TextureLight* pGO = (GraphicsObject_TextureLight*)pGlobalGO->GetGraphicsObject();
		//	pGO->SetTexture(Texture::Name::WAR_BEAR_POLAR);
		//}

		AnimController* pHumanoid1 = AnimControllerMan::Find(AnimController::Name::Humanoid);
		AnimController* pHumanoid2 = AnimControllerMan::Find(AnimController::Name::Test);

		// Change speed of animation
		if (pKey->GetKeyState(Keyboard::KEY_EQUAL)) {
			pHumanoid1->SpeedUp();
			pHumanoid2->SpeedUp();
		}
		else if (pKey->GetKeyState(Keyboard::KEY_MINUS)) {
			pHumanoid1->SlowDown();
			pHumanoid2->SlowDown();
		}

		static bool periodPressed = false;
		if (pKey->GetKeyState(Keyboard::KEY_PERIOD))
		{
			if (!periodPressed) {

				Clip* pClip1;
				Clip* pClip2;

				if (humanClip == Clip::Name::HUMAN_RUN)
				{
					pClip1 = ClipMan::Find(Clip::Name::HUMAN_WALK);
					humanClip = Clip::Name::HUMAN_WALK;
					pClip2 = ClipMan::Find(Clip::Name::HUMAN_WALK_NOT_COMPRESSED);
					humanNCClip = Clip::Name::HUMAN_WALK_NOT_COMPRESSED;

					float ratio = (float)ClipMan::Find(Clip::Name::HUMAN_WALK_NOT_COMPRESSED)->clipSize / (float)ClipMan::Find(Clip::Name::HUMAN_WALK)->clipSize;
					std::string s = "1 : " + std::to_string(ratio);
					FontMan::Find(Font::Name::TestMessage1)->UpdateMessage(s.c_str());
				}
				else
				{
					pClip1 = ClipMan::Find(Clip::Name::HUMAN_RUN);
					humanClip = Clip::Name::HUMAN_RUN;
					pClip2 = ClipMan::Find(Clip::Name::HUMAN_RUN_NOT_COMPRESSED);
					humanNCClip = Clip::Name::HUMAN_RUN_NOT_COMPRESSED;

					float ratio = (float)ClipMan::Find(Clip::Name::HUMAN_RUN_NOT_COMPRESSED)->clipSize / (float)ClipMan::Find(Clip::Name::HUMAN_RUN)->clipSize;
					std::string s = "1 : " + std::to_string(ratio);
					FontMan::Find(Font::Name::TestMessage1)->UpdateMessage(s.c_str());
				}

				pHumanoid1->SetClip(pClip1);
				pHumanoid2->SetClip(pClip2);
			}

			periodPressed = true;
		}
		else {
			periodPressed = false;
		}

		static bool semiPressed = false;
		if (pKey->GetKeyState(Keyboard::KEY_SEMICOLON))
		{
			if (!semiPressed) {

				Clip* pClip1;
				Clip* pClip2;

				if (humanClip == Clip::Name::HUMAN_RUN)
				{
					pClip1 = ClipMan::Find(Clip::Name::HUMAN_WALK);
					humanClip = Clip::Name::HUMAN_WALK;
					pClip2 = ClipMan::Find(Clip::Name::HUMAN_WALK_NOT_COMPRESSED);
					humanNCClip = Clip::Name::HUMAN_WALK_NOT_COMPRESSED;

					float ratio = (float)ClipMan::Find(Clip::Name::HUMAN_WALK_NOT_COMPRESSED)->clipSize / (float)ClipMan::Find(Clip::Name::HUMAN_WALK)->clipSize;
					std::string s = "1 : " + std::to_string(ratio);
					FontMan::Find(Font::Name::TestMessage1)->UpdateMessage(s.c_str());
				}
				else
				{
					pClip1 = ClipMan::Find(Clip::Name::HUMAN_RUN);
					humanClip = Clip::Name::HUMAN_RUN;
					pClip2 = ClipMan::Find(Clip::Name::HUMAN_RUN_NOT_COMPRESSED);
					humanNCClip = Clip::Name::HUMAN_RUN_NOT_COMPRESSED;

					float ratio = (float)ClipMan::Find(Clip::Name::HUMAN_RUN_NOT_COMPRESSED)->clipSize / (float)ClipMan::Find(Clip::Name::HUMAN_RUN)->clipSize;
					std::string s = "1 : " + std::to_string(ratio);
					FontMan::Find(Font::Name::TestMessage1)->UpdateMessage(s.c_str());
				}

				pHumanoid1->TransitionToClip(pClip1);
				pHumanoid2->TransitionToClip(pClip2);
			}

			semiPressed = true;
		}
		else {
			semiPressed = false;
		}

		AnimController* pTeddy = AnimControllerMan::Find(AnimController::Name::Teddy);
		static bool commaPressed = false;
		if (pKey->GetKeyState(Keyboard::KEY_COMMA))
		{
			if (!commaPressed) {
				Clip* pClip;

				if (teddyClip == Clip::Name::TEDDY_WALK)
				{
					pClip = ClipMan::Find(Clip::Name::TEDDY_IDLE);
					teddyClip = Clip::Name::TEDDY_IDLE;
				}
				else
				{
					pClip = ClipMan::Find(Clip::Name::TEDDY_WALK);
					teddyClip = Clip::Name::TEDDY_WALK;
				}

				pTeddy->SetClip(pClip);
			}

			commaPressed = true;
		}
		else {
			commaPressed = false;
		}
	}
}
// --- End of File ---
