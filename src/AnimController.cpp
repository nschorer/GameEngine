//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "AnimController.h"
#include "Anim.h"
#include "CustomModel.h"

namespace Azul
{

	AnimController::~AnimController()
	{
		delete poAnim;
		delete poTimerControl;
	}

	AnimController::AnimController(Name _name, const char* const pAnimFileName, AnimTime delta, CustomModel* pModel)
		:name(_name)
	{
		// Animation object
		this->poAnim = new Anim(pAnimFileName, pModel);
		assert(this->poAnim);

		// TimerController
		AnimTime maxTime = poAnim->FindMaxTime();
		AnimTime minTime = AnimTime(AnimTime::Duration::ZERO);
		this->poTimerControl = new TimerController(delta, minTime, maxTime);
		assert(this->poTimerControl);
	}

	AnimController::AnimController(Name _name, Clip* _pClip, AnimTime delta)
		:name(_name)
	{
		// Animation object
		this->poAnim = new Anim(_pClip);
		assert(this->poAnim);

		// TimerController
		AnimTime maxTime = poAnim->FindMaxTime();
		AnimTime minTime = AnimTime(AnimTime::Duration::ZERO);
		this->poTimerControl = new TimerController(delta, minTime, maxTime);
		assert(this->poTimerControl);
	}

	void AnimController::SpeedUp()
	{
		AnimTime tmp = this->poTimerControl->GetDeltaTime();
		tmp += 0.005f * AnimTime(AnimTime::Duration::NTSC_30_FRAME);
		this->poTimerControl->SetDeltaTime(tmp);
	}

	void AnimController::SlowDown()
	{
		AnimTime tmp = this->poTimerControl->GetDeltaTime();
		tmp -= 0.005f * AnimTime(AnimTime::Duration::NTSC_30_FRAME);
		this->poTimerControl->SetDeltaTime(tmp);
	}

	void AnimController::Update()
	{
		this->poTimerControl->Update();

		// interpolate all bones
		this->poAnim->Animate(this->poTimerControl->GetCurrTime());

		// convert locals to worlds
		this->poAnim->SetBoneWorlds(this->poTimerControl->GetCurrTime());
	}
	void AnimController::SetClip(Clip* pClip)
	{
		this->poAnim->SetClip(pClip);
		/*AnimTime maxTime = poAnim->FindMaxTime();
		this->poTimerControl->SetMaxTime(maxTime);*/
	}

	void AnimController::TransitionToClip(Clip* pClip)
	{
		this->poAnim->TransitionToClip(pClip);
		/*AnimTime maxTime = pClip->GetTotalTime();
		this->poTimerControl->SetMaxTime(maxTime);*/
	}

	AnimController::Name AnimController::GetName()
	{
		return name;
	}
}

// --- End of File ---
