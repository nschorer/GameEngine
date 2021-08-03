//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Mixer.h"
#include "MathApp.h"
#include "ShaderMan.h"

//#define DEBUG_MIXER

namespace Azul
{
	Mixer::Mixer()
	{
		pSSBO_in1 = new SSBO();
		pSSBO_in2 = new SSBO();
		pSSBO_out = new SSBO();
	}

	Mixer::~Mixer()
	{
		delete pSSBO_in1;
		delete pSSBO_in2;
		delete pSSBO_out;
	}


	Mixer* Mixer::privGetInstance()
	{
		static Mixer instance;
		return &instance;
	}

	

	void Mixer::Create()
	{
		Mixer* pInstance = Mixer::privGetInstance();
		assert(pInstance);
	}

	void Mixer::Destroy()
	{
	}

	void Mixer::Blend(Bone *pResult,
		const Bone *pA,
		const Bone *pB,
		const float tS,
		int numBones)
	{
		// Interpolate to tS time, for all bones
		for (int i = 0; i < numBones; i++)
		{
			// interpolate ahoy!
			VectApp::Lerp(pResult->T, pA->T, pB->T, tS);
			QuatApp::Slerp(pResult->Q, pA->Q, pB->Q, tS);
			VectApp::Lerp(pResult->S, pA->S, pB->S, tS);

#ifdef DEBUG_MIXER
			Trace::out("\nBone(CPU): %i\n", i);
			Trace::out("T: (%f, %f, %f)\n", pResult->T.x(), pResult->T.y(), pResult->T.z());
			Trace::out("Q: (%f, %f, %f)\n", pResult->Q.qx(), pResult->Q.qy(), pResult->Q.qz());
			Trace::out("S: (%f, %f, %f)\n", pResult->S.x(), pResult->S.y(), pResult->S.z());
			Trace::out("\n");
#endif

			// advance the pointer
			pA++;
			pB++;
			pResult++;
		}

	}

	void Mixer::BlendCompute(Bone* pResult, const Bone* pA, const Bone* pB, const float tS, const int numBones)
	{

		

		Mixer* pMixer = privGetInstance();
		SSBO*& in1 = pMixer->pSSBO_in1;
		SSBO*& in2 = pMixer->pSSBO_in2;
		SSBO*& out = pMixer->pSSBO_out;

		in1->Bind(0);
		in2->Bind(1);
		out->Bind(2);

		unsigned int num = (unsigned int)numBones;

		in1->Set(num, sizeof(Bone), (void*)pA);
		in2->Set(num, sizeof(Bone), (void*)pB);
		out->Set(num, sizeof(Bone), pResult);

		ShaderObject* pComputeShader = ShaderMan::Find(ShaderObject::Name::COMPUTE_MIXER);
		assert(pComputeShader);
		pComputeShader->SetActive();

		glUniform1f(pComputeShader->GetLocation("ts"), tS);
		glUniform1i(pComputeShader->GetLocation("boneCount"), numBones);

		// hard code to groups of 20 for now
		unsigned int numBatches = num / 20 + (num%20 != 0);
		pComputeShader->Dispatch(numBatches, 1, 1);

		// Block Waiting for stages to complete
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

		// Retrieve the result from the compute shader and shove it in the skeleton
		memcpy(pResult, out->Map(SSBO::Access::READ_ONLY), sizeof(Bone) * num);

		// needed?
		out->Unmap();

#ifdef DEBUG_MIXER

		for (int i = 0; i < numBones; i++)
		{
			Trace::out("\nBone(GPU): %i\n", i);
			Trace::out("T: (%f, %f, %f)\n", pResult[i].T.x(), pResult[i].T.y(), pResult[i].T.z());
			Trace::out("Q: (%f, %f, %f)\n", pResult[i].Q.qx(), pResult[i].Q.qy(), pResult[i].Q.qz());
			Trace::out("S: (%f, %f, %f)\n", pResult[i].S.x(), pResult[i].S.y(), pResult[i].S.z());
			Trace::out("\n");

		}

#endif
		
	}

	void Mixer::BlendCompute(SSBO* pKey_A, SSBO* pKey_B, SSBO* pResult_GPU, const float tS, const int numBones)
	{
		assert(pKey_A != nullptr);
		assert(pKey_B != nullptr);
		assert(pResult_GPU != nullptr);

		// Lock and load the shader...
		ShaderObject* pComputeShader = ShaderMan::Find(ShaderObject::Name::COMPUTE_MIXER);

		pComputeShader->SetActive();

		pKey_A->Bind(0);
		pKey_B->Bind(1);
		pResult_GPU->Bind(2);

		glUniform1f(pComputeShader->GetLocation("ts"), tS);
		glUniform1i(pComputeShader->GetLocation("boneCount"), numBones);

		unsigned int numBatches = numBones / 20 + (unsigned int)(numBones % 20 != 0);
		pComputeShader->Dispatch(numBatches, 1, 1);

		// Block Waiting for stages to complete
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	}

	

}

// --- End of File ---
