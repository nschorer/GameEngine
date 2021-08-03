
#include "AnimTransition.h"
#include "Clip.h"
#include "Mixer.h"
#include "TeddyModel.h"
#include "ShaderMan.h"
#include "MathApp.h"

namespace Azul
{
	extern TeddyModel* pTeddyModel;

	AnimTransition::AnimTransition(Clip* pClipA, Clip* pClipB)
		: pSSBO_A(nullptr),
		pSSBO_B(nullptr),
		poSSBO_Result(nullptr),
		poBoneWorld_SBO(nullptr),
		numBones(-1),
		tS(0.0f),
		StartEnable(false)
	{
		assert(pClipA);
		assert(pClipB);

		this->pSSBO_A = pClipA->poSSBO_Result;
		this->pSSBO_B = pClipB->poSSBO_Result;

		assert(this->pSSBO_A);
		assert(this->pSSBO_A);

		assert(pClipA->numBones == pClipB->numBones);

		this->numBones = (int)pClipA->numBones;

		this->poSSBO_Result = new SSBO();
		Bone* pBone = new Bone[(unsigned int)numBones]();
		this->poSSBO_Result->Set((unsigned int)numBones, sizeof(Bone), pBone);
		delete[] pBone;

		this->poBoneWorld_SBO = new SSBO();		
		Matrix *pResultMatrix = new Matrix[(unsigned int)numBones]();
		this->poBoneWorld_SBO->Set((unsigned int)numBones, sizeof(Matrix), pResultMatrix);
		delete[] pResultMatrix;

		pSSBO_Hierarchy = pClipA->poSSBO_Hierarchy;
	}

	AnimTransition::~AnimTransition()
	{
		delete this->poSSBO_Result;
		delete this->poBoneWorld_SBO;
	}

	void AnimTransition::StartTransition()
	{
		this->StartEnable = true;
	}

	void AnimTransition::privTransitionTime()
	{
		if (this->StartEnable)
		{
			float t_delta = 0.03f;
			float b = 1.0f;
			float a = 0.0f;
			static float t = 0.0f;
			t += t_delta;
			if (t > 1.0f)
			{
				t = 1.0f;
			}

			this->tS = a + t * (b - a);
		}
	}

	void AnimTransition::Update()
	{
		this->privTransitionTime();

		Mixer::BlendCompute(this->pSSBO_A, this->pSSBO_B, this->poSSBO_Result, tS, this->numBones);

		// ------------------------------------------------
		// Compute local to world shader - move this
		// ------------------------------------------------

		// Lock and load the shader...
		ShaderObject* pComputeShader = ShaderMan::Find(ShaderObject::Name::COMPUTE_LOCAL_TO_WORLD);

		pComputeShader->SetActive();

		// need a clean way to store and hold this data...
		SSBO* pBoneLocal = this->poSSBO_Result;

		pBoneLocal->Bind(0);
		pSSBO_Hierarchy->Bind(1);
		this->poBoneWorld_SBO->Bind(2);

		// Dispatch
		pComputeShader->Dispatch(1, 1, 1);

		// Block Waiting for stages to complete
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	}

}