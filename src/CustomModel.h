#ifndef CUSTOM_MODEL_H
#define CUSTOM_MODEL_H

#include "Model.h"
#include "TextureMan.h"
#include "ChunkHeader.h"

//struct ChunkHeader;
//class VertsHeader;
//class TrisHeader;
//class TextureHeader;
//class SphereHeader;

namespace Azul
{
	class CustomModel : public Model
	{
	public:
		CustomModel(const char* const pModelFileName);

		CustomModel() = delete;
		CustomModel(const CustomModel&) = delete;
		CustomModel& operator = (CustomModel&) = delete;
		virtual ~CustomModel();

		

		

	private:
		virtual void privCreateVAO(const char* const pModelFileName) override;

		void privParseVerts(VertsHeader* vHdr, unsigned char*& p);
		void privParseTris(TrisHeader* triHdr, unsigned char*& p);
		void privParseText(TextureHeader* textHdr, unsigned char*& p);
		void privParseSphere(SphereHeader* sHdr, unsigned char*& p);
		void privParseBoneWeights(BoneWeightHeader* bHdr, unsigned char*& p);
		void privParseBoneIdxs(BoneIdxHeader* bHdr, unsigned char*& p);
		void privParseInvMats(InvMatHeader* imHdr, unsigned char*& p);

	};
}

#endif

// --- End of File ---
