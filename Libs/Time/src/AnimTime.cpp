//--------------------------------------------------------------
// Copyright 2021, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "AnimTime.h"

namespace Azul
{
	//---------------------------------------------------------------------------
	// FRIENDS:
	//---------------------------------------------------------------------------
	const AnimTime operator*(const float ratio, const AnimTime &rhs)
	{
		return(AnimTime(static_cast<AnimTime::Representation>((double)ratio * (double)rhs.privRawAnimTime)));
	}

	const AnimTime operator*(const int ratio, const AnimTime &rhs)
	{
		return(AnimTime(ratio * rhs.privRawAnimTime));
	}

	//---------------------------------------------------------------------------
	// CONSTRUCTORS:
	//---------------------------------------------------------------------------

	AnimTime::AnimTime(const Duration duration) : privRawAnimTime(0)
	{
		// IMPORTANT: This is private information that is SUBJECT TO CHANGE!
		//
		// Currently: 1 second = 30000000
		//            1 us     = 30
		//            1 ms     = 30000
		//            1 NTSC   = 500000
		//            1 PAL    = 600000
		//
		// At 32 bits, this gives us a range of roughly -20 to 20 hours.
		const AnimTime::Representation ONE_RAW_SECOND = 30000000;

		switch (duration)
		{
		case Duration::ZERO:
			this->privRawAnimTime = 0;
			break;

		case Duration::NTSC_FRAME:
			this->privRawAnimTime = ONE_RAW_SECOND / 60;
			break;

		case Duration::NTSC_30_FRAME:
			this->privRawAnimTime = 2 * ONE_RAW_SECOND / 60;
			break;

		case Duration::PAL_FRAME:
			this->privRawAnimTime = ONE_RAW_SECOND / 50;
			break;

		case Duration::ONE_SECOND:
			this->privRawAnimTime = ONE_RAW_SECOND;
			break;

		case Duration::ONE_MILLISECOND:
			this->privRawAnimTime = ONE_RAW_SECOND / 1000;
			break;

		case Duration::ONE_MICROSECOND:
			this->privRawAnimTime = ONE_RAW_SECOND / 1000000;
			break;

		case Duration::ONE_MINUTE:
			this->privRawAnimTime = 60 * ONE_RAW_SECOND;
			break;

		case Duration::ONE_HOUR:
			this->privRawAnimTime = 60 * 60 * ONE_RAW_SECOND;
			break;

		case Duration::MIN:
			this->privRawAnimTime = std::numeric_limits< AnimTime::Representation >::min();
			break;

		case Duration::MAX:
			this->privRawAnimTime = std::numeric_limits< AnimTime::Representation >::max();
			break;

		case Duration::DWORD:
			assert(false);
			this->privRawAnimTime = 0;
			break;

		default:
			assert(false);
			this->privRawAnimTime = 0;
			break;
		}
	}

	//---------------------------------------------------------------------------
	// COMPARISONS:
	//---------------------------------------------------------------------------
	bool AnimTime::operator==(const AnimTime &rhs) const
	{
		return(this->privRawAnimTime == rhs.privRawAnimTime);
	}

	bool AnimTime::operator!=(const AnimTime &rhs) const
	{
		return(this->privRawAnimTime != rhs.privRawAnimTime);
	}

	bool AnimTime::operator<(const AnimTime &rhs) const
	{
		return(this->privRawAnimTime < rhs.privRawAnimTime);
	}

	bool AnimTime::operator<=(const AnimTime &rhs) const
	{
		return(this->privRawAnimTime <= rhs.privRawAnimTime);
	}

	bool AnimTime::operator>(const AnimTime &rhs) const
	{
		return(this->privRawAnimTime > rhs.privRawAnimTime);
	}

	bool AnimTime::operator>=(const AnimTime &rhs) const
	{
		return(this->privRawAnimTime >= rhs.privRawAnimTime);
	}

	//---------------------------------------------------------------------------
	// NEGATION / ADDITION / SUBTRACTION:
	//---------------------------------------------------------------------------
	const AnimTime AnimTime::operator-() const
	{
		return(AnimTime(-this->privRawAnimTime));
	}

	const AnimTime AnimTime::operator+(const AnimTime &rhs) const
	{
		// !!! FIXME: Overflow checks
		return(AnimTime(this->privRawAnimTime + rhs.privRawAnimTime));
	}

	const AnimTime AnimTime::operator-(const AnimTime &rhs) const
	{
		// !!! FIXME: Overflow checks
		return(AnimTime(this->privRawAnimTime - rhs.privRawAnimTime));
	}

	AnimTime &AnimTime::operator+=(const AnimTime &rhs)
	{
		// !!! FIXME: Overflow checks
		this->privRawAnimTime += rhs.privRawAnimTime;
		return(*this);
	}

	AnimTime &AnimTime::operator-=(const AnimTime &rhs)
	{
		// !!! FIXME: Overflow checks
		this->privRawAnimTime -= rhs.privRawAnimTime;
		return(*this);
	}

	//---------------------------------------------------------------------------
	// MULTIPLICATION:
	//---------------------------------------------------------------------------
	const AnimTime AnimTime::operator*(const float ratio) const
	{
		// !!! FIXME: Overflow checks
		return(AnimTime(static_cast<Representation>((double)ratio * (double)this->privRawAnimTime)));
	}

	const AnimTime AnimTime::operator*(const int ratio) const
	{
		// !!! FIXME: Overflow checks
		return(AnimTime(this->privRawAnimTime * ratio));
	}

	AnimTime &AnimTime::operator*=(const float ratio)
	{
		// !!! FIXME: Overflow checks
		this->privRawAnimTime = static_cast<Representation>((double)ratio * (double)this->privRawAnimTime);
		return(*this);
	}

	AnimTime &AnimTime::operator*=(const int ratio)
	{
		// !!! FIXME: Overflow checks
		this->privRawAnimTime *= ratio;
		return(*this);
	}


	//---------------------------------------------------------------------------
	// DIVISION:
	//---------------------------------------------------------------------------
	float AnimTime::operator/(const AnimTime &denominator) const
	{
		// !!! FIXME: Divide by zero.
		return(static_cast<float>(this->privRawAnimTime) / (float)denominator.privRawAnimTime);
	}


	const AnimTime AnimTime::operator/(const float denominator) const
	{
		// !!! FIXME: Divide by zero.
		return(AnimTime(static_cast<Representation>((float)this->privRawAnimTime / denominator)));
	}

	const AnimTime AnimTime::operator/(const int denominator) const
	{
		assert((float)denominator != 0.0f);
		return(AnimTime(this->privRawAnimTime / denominator));
	}

	AnimTime &AnimTime::operator/=(const float denominator)
	{
		assert(denominator != 0.0f);
		this->privRawAnimTime = (Representation)((double)this->privRawAnimTime / (double)denominator);
		return(*this);
	}

	AnimTime &AnimTime::operator/=(const int denominator)
	{
		assert((float)denominator != 0.0f);
		this->privRawAnimTime /= denominator;
		return(*this);
	}

	//---------------------------------------------------------------------------
	// DIVISION:
	//---------------------------------------------------------------------------
	int 	AnimTime::Quotient(const AnimTime &numerator, const AnimTime &denominator)
	{
		// !!! FIXME: Divide by zero & check range
		assert((float)denominator.privRawAnimTime != 0.0f);
		return(static_cast<int> (numerator.privRawAnimTime / denominator.privRawAnimTime));
	}

	const AnimTime 	AnimTime::Remainder(const AnimTime &numerator, const AnimTime &denominator)
	{
		return(AnimTime(numerator.privRawAnimTime % denominator.privRawAnimTime));
	}

	//---------------------------------------------------------------------------
	// PRIVATE IMPLEMENTATION:
	//---------------------------------------------------------------------------
	AnimTime::AnimTime(const AnimTime::Representation rawTime) : privRawAnimTime(rawTime)
	{
	}

}

// ---  End of File ---
