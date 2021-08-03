#include "AnimControllerMan.h"

namespace Azul {
	AnimController* AnimControllerMan::Add(AnimController::Name name, const char* const filename, CustomModel* pModel)
	{
		AnimControllerMan* pMan = AnimControllerMan::privGetInstance();
		assert(pMan);

		AnimTime delta = 0.2f * AnimTime(AnimTime::Duration::NTSC_30_FRAME);
		AnimController* pAnimController = new AnimController(name, filename, delta, pModel);

		pMan->privAddToFront(pAnimController, pMan->active);

		return pAnimController;
	}

	AnimController* AnimControllerMan::Add(AnimController::Name name, Clip* pClip)
	{
		AnimControllerMan* pMan = AnimControllerMan::privGetInstance();
		assert(pMan);

		AnimTime delta = 0.2f * AnimTime(AnimTime::Duration::NTSC_30_FRAME);
		AnimController* pAnimController = new AnimController(name, pClip, delta);

		pMan->privAddToFront(pAnimController, pMan->active);

		return pAnimController;
	}

	AnimController* AnimControllerMan::Find(AnimController::Name name)
	{
		AnimControllerMan* pMan = AnimControllerMan::privGetInstance();
		assert(pMan);

		AnimControllerLink* pLink = pMan->active;
		AnimController* pTmp = nullptr;

		while (pLink != nullptr)
		{
			pTmp = (AnimController*)pLink;
			if (pTmp->GetName() == name)
			{
				return pTmp;
			}
			pLink = pLink->next;
		}

		assert(false);
		return nullptr;
	}

	void AnimControllerMan::Update()
	{
		AnimControllerMan* pMan = AnimControllerMan::privGetInstance();
		assert(pMan);

		AnimControllerLink* pLink = pMan->active;

		while (pLink != nullptr)
		{
			AnimController* pTmp = (AnimController*)pLink;
			pTmp->Update();
			pLink = pLink->next;
		}
	}

	void AnimControllerMan::Remove(AnimController::Name name)
	{
		AnimControllerMan* pMan = AnimControllerMan::privGetInstance();
		assert(pMan);

		AnimControllerLink* pLink = pMan->active;
		AnimController* pTmp = nullptr;

		while (pLink != nullptr)
		{
			pTmp = (AnimController*)pLink;
			if (pTmp->GetName() == name)
			{
				pMan->privRemove(pTmp, pMan->active);
				delete pTmp;
				return;
			}
			pLink = pLink->next;
		}

		assert(false);
	}

	void AnimControllerMan::Destroy()
	{
		AnimControllerMan* pMan = AnimControllerMan::privGetInstance();
		assert(pMan);

		AnimControllerLink* pLink = pMan->active;

		while (pLink != nullptr)
		{
			AnimControllerLink* pTmp = pLink;
			pLink = pLink->next;
			pMan->privRemove(pTmp, pMan->active);
			delete pTmp;
		}
	}
	void AnimControllerMan::Create()
	{
		AnimControllerMan* pMan = AnimControllerMan::privGetInstance();
		assert(pMan);
	}
	AnimControllerMan::AnimControllerMan()
	{
		this->active = nullptr;
	}

	AnimControllerMan* AnimControllerMan::privGetInstance()
	{
		// This is where its actually stored (BSS section)
		static AnimControllerMan animControllerMan;
		return &animControllerMan;
	}

	void AnimControllerMan::privAddToFront(AnimControllerLink* node, AnimControllerLink*& head)
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

	void AnimControllerMan::privRemove(AnimControllerLink* pNode, AnimControllerLink*& poHead)
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