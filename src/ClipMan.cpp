#include "ClipMan.h"
#include "ChunkHeader.h"
#include "File.h"
#include "PackageHeader.h"
#include "Skeleton.h"

namespace Azul
{

    void ClipMan::Add(Clip* pClip)
    {
        ClipMan* pMan = ClipMan::privGetInstance();
        assert(pMan);

        pMan->privAddToFront(pClip, pMan->active);
    }

    Clip* ClipMan::Find(Clip::Name name)
    {
        ClipMan* pMan = ClipMan::privGetInstance();
        assert(pMan);

        ClipLink* pLink = pMan->active;
        Clip* pTmp = nullptr;

        while (pLink != nullptr)
        {
            pTmp = (Clip*)pLink;
            if (pTmp->GetName() == name)
            {
                return pTmp;
            }
            pLink = pLink->next;
        }

        assert(false);
        return nullptr;
    }

    void ClipMan::Destroy()
    {
        ClipMan* pMan = ClipMan::privGetInstance();
        assert(pMan);

        ClipLink* pLink = pMan->active;

        while (pLink != nullptr)
        {
            ClipLink* pTmp = pLink;
            pLink = pLink->next;
            pMan->privRemove(pTmp, pMan->active);
            delete pTmp;
        }
    }

    void ClipMan::Create()
    {
        ClipMan* pMan = ClipMan::privGetInstance();
        assert(pMan);
    }

    void ClipMan::ImportClips(const char* const pAnimFileName)
    {
        // File stuff
        File::Handle fh;
        File::Error  ferror;

        // READ the data from the file ONLY

        ferror = File::Open(fh, pAnimFileName, File::Mode::READ);
        assert(ferror == File::Error::SUCCESS);

        // Read the Package Header to find out how much is in the file
        PackageHeader pHdr;

        ferror = File::Read(fh, &pHdr, sizeof(PackageHeader));
        assert(ferror == File::Error::SUCCESS);

        unsigned int numChunks = pHdr.numChunks;
        unsigned int packageSize = pHdr.packageSize;
        char animName[PackageHeader::PACKAGE_NAME_SIZE];
        strcpy_s(animName, PackageHeader::PACKAGE_NAME_SIZE, pHdr.packageName);
        char animVersion[PackageHeader::PACKAGE_VERSION_SIZE];
        strcpy_s(animVersion, PackageHeader::PACKAGE_VERSION_SIZE, pHdr.versionString);

        unsigned char* buffer = new unsigned char[packageSize];

        ferror = File::Read(fh, buffer, packageSize);
        unsigned char* p = (unsigned char*)buffer;
        unsigned char* bufferBottom = p + packageSize;

        int numBones = 0;

        // Read all the chunks -- each chunk is an animation clip
        for (unsigned int i = numChunks; i > 0; i--) {
            // Figure out what kind of chunk we're dealing with

            ChunkHeader* cHdr = (ChunkHeader*)p;

            switch (cHdr->type) {


            case ChunkType::SKEL_TYPE:
            {
                SkeletonHeader* skelHdr = (SkeletonHeader*)cHdr;
                //privParseSkel(skelHdr, p);

                numBones = skelHdr->numBones;

                p += sizeof(SkeletonHeader);
                p += skelHdr->chunkSize;
                break;
            }

            case ChunkType::H_TABLE_TYPE:
            {
                HTableHeader* hHdr = (HTableHeader*)cHdr;
                //privParseHTable(hHdr, p);
                p += sizeof(HTableHeader);
                p += hHdr->chunkSize;
                break;
            }

            case ChunkType::CLIP_TYPE:
            {
                ClipHeader* clipHdr = (ClipHeader*)cHdr;
                privParseClip(clipHdr, p, numBones);
                break;
            }
            case ChunkType::NORMS_TYPE:
            case ChunkType::SPHERE_TYPE:
            case ChunkType::TEXTURE_TYPE:
            case ChunkType::TRIS_TYPE:
            case ChunkType::UV_TYPE:
            case ChunkType::VERTS_TYPE:
            case ChunkType::BONE_WEIGHT_TYPE:
            case ChunkType::BONE_IDX_TYPE:
            case ChunkType::INV_MAT_TYPE:

            default:
            {
                assert(false); // Not implemented 
                break;
            }
            }
        }

        assert(p == bufferBottom);
        delete[] buffer;
    
    }

    void ClipMan::ImportClips2(const char* const pAnimFileName, Model* pModel)
    {
        // File stuff
        File::Handle fh;
        File::Error  ferror;

        AZUL_UNUSED_VAR(pModel);

        // READ the data from the file ONLY

        ferror = File::Open(fh, pAnimFileName, File::Mode::READ);
        assert(ferror == File::Error::SUCCESS);

        // Read the Package Header to find out how much is in the file
        PackageHeader pHdr;

        ferror = File::Read(fh, &pHdr, sizeof(PackageHeader));
        assert(ferror == File::Error::SUCCESS);

        unsigned int numChunks = pHdr.numChunks;
        unsigned int packageSize = pHdr.packageSize;
        char animName[PackageHeader::PACKAGE_NAME_SIZE];
        strcpy_s(animName, PackageHeader::PACKAGE_NAME_SIZE, pHdr.packageName);
        char animVersion[PackageHeader::PACKAGE_VERSION_SIZE];
        strcpy_s(animVersion, PackageHeader::PACKAGE_VERSION_SIZE, pHdr.versionString);

        unsigned char* buffer = new unsigned char[packageSize];

        ferror = File::Read(fh, buffer, packageSize);
        unsigned char* p = (unsigned char*)buffer;
        unsigned char* bufferBottom = p + packageSize;

        int numBones = 0;
        int hTableSize = 0;
        int* hTable = nullptr;
        SkeletonData* pSkeletonData = nullptr;

        // Read all the chunks -- each chunk is an animation clip
        for (unsigned int i = numChunks; i > 0; i--) {
            // Figure out what kind of chunk we're dealing with

            ChunkHeader* cHdr = (ChunkHeader*)p;

            switch (cHdr->type) {


            case ChunkType::SKEL_TYPE:
            {
                SkeletonHeader* skelHdr = (SkeletonHeader*)cHdr;
                privParseSkeleton(skelHdr, p, numBones, pSkeletonData);
                break;
            }

            case ChunkType::H_TABLE_TYPE:
            {
                HTableHeader* hHdr = (HTableHeader*)cHdr;
                privParseHTable(hHdr, p, numBones, hTable, hTableSize);
                break;
            }

            case ChunkType::CLIP_TYPE:
            {
                ClipHeader* clipHdr = (ClipHeader*)cHdr;
                privParseClip(clipHdr, p, pSkeletonData, numBones, hTable, hTableSize, pModel);
                break;
            }
            case ChunkType::NORMS_TYPE:
            case ChunkType::SPHERE_TYPE:
            case ChunkType::TEXTURE_TYPE:
            case ChunkType::TRIS_TYPE:
            case ChunkType::UV_TYPE:
            case ChunkType::VERTS_TYPE:
            case ChunkType::BONE_WEIGHT_TYPE:
            case ChunkType::BONE_IDX_TYPE:
            case ChunkType::INV_MAT_TYPE:

            default:
            {
                assert(false); // Not implemented 
                break;
            }
            }
        }

        assert(p == bufferBottom);

        delete[] hTable;
        delete[] buffer;
    }

    ClipMan::ClipMan()
    {
        this->active = nullptr;
    }

    ClipMan* ClipMan::privGetInstance()
    {
        static ClipMan pMan;
        return &pMan;
    }

    void ClipMan::privParseClip(ClipHeader* clipHdr, unsigned char*& p, int numBones)
    {
        assert(false);

        assert(numBones > 0); // this means skeleton MUST be processed before clips
        assert(clipHdr->numFrames > 0);
        //assert(animHdr->TotalTime > 0);

        int numFrames = clipHdr->numFrames;
        float totalTime = clipHdr->totalTime;
        AnimTime TotalTime = Azul::AnimTime(Azul::AnimTime::Duration::ONE_SECOND) * (totalTime / 30.0f);

        p += sizeof(ClipHeader); // increment p

        // Get this from the AnimMan
        Clip* tmp = new Clip(p, (Clip::Name)clipHdr->name, numBones, numFrames, TotalTime, clipHdr->chunkSize);

        ClipMan::Add(tmp);

        p += clipHdr->chunkSize;
    }

    void ClipMan::privParseSkeleton(SkeletonHeader* skelHdr, unsigned char*& p, int& numBones, SkeletonData*& pSkeletonData)
    {
        assert(skelHdr->numBones > 0);

        numBones = skelHdr->numBones;

        p += sizeof(SkeletonHeader);

        pSkeletonData = (SkeletonData*)p;

        p += skelHdr->chunkSize;
    }

    void ClipMan::privParseHTable(HTableHeader* hHdr, unsigned char*& p, int numBones, int*& hTable, int& numInts)
    {
        int depth = hHdr->depth;

        p += sizeof(HTableHeader);

        unsigned int num = (unsigned int)(1 + 1 + depth * numBones);
        numInts = (int)num;

        // Delete this after import is complete so we can make sure all clips get it
        hTable = new int[num * sizeof(int)];

        memcpy(hTable, &depth, sizeof(int));
        memcpy(hTable + 1, &numBones, sizeof(int));
        memcpy(hTable + 2, p, sizeof(int) * depth * numBones);

        p += hHdr->chunkSize;
    }

    void ClipMan::privParseClip(ClipHeader* clipHdr, unsigned char*& p, SkeletonData* pSkeletonData, int numBones, int* hTable, int numInts, Model* _pModel)
    {
        assert(numBones > 0); // this means skeleton MUST be processed before clips
        assert(clipHdr->numFrames > 0);
        //assert(animHdr->TotalTime > 0);

        int numFrames = clipHdr->numFrames;
        float totalTime = clipHdr->totalTime;
        AnimTime TotalTime = Azul::AnimTime(Azul::AnimTime::Duration::ONE_SECOND) * (totalTime / 30.0f);

        p += sizeof(ClipHeader); // increment p

        // Get this from the AnimMan
        Clip* tmp = new Clip(p, (Clip::Name)clipHdr->name, numBones, numFrames, hTable, numInts, TotalTime, clipHdr->chunkSize);
        tmp->poSkeleton = new Skeleton(pSkeletonData, numBones); // eventually make a skeleton manager so we don't need multiple copies of this

        assert(_pModel->numBones == (unsigned int)numBones);
        tmp->pModel = _pModel;

        ClipMan::Add(tmp);

        p += clipHdr->chunkSize;
    }


    void ClipMan::privAddToFront(ClipLink* node, ClipLink*& head)
    {
        assert(node != 0);

        if (head == 0)
        {
            head = node;
            node->next = 0;
            node->prev = 0;
        }
        else
        {
            node->next = head;
            head->prev = node;
            head = node;
        }
    }

    void ClipMan::privRemove(ClipLink* pNode, ClipLink*& poHead)
    {
        assert(pNode);

        if (pNode->prev != nullptr)
        {	// middle or last node
            pNode->prev->next = pNode->next;
        }
        else
        {  // first
            poHead = pNode->next;
        }

        if (pNode->next != nullptr)
        {	// middle node
            pNode->next->prev = pNode->prev;
        }
    }

}