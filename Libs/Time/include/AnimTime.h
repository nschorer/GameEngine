//--------------------------------------------------------------
// Copyright 2021, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef ANIM_TIME_H
#define ANIM_TIME_H

//---------------------------------------------------------------------------
// CLASS DECLARATION:
//---------------------------------------------------------------------------

namespace Azul
{
	class AnimTime
	{
	public:

		enum class Duration
		{
			// For constructing a AnimTime of zero. 
			ZERO,

			// For constructing one NTSC 60 hz frame of AnimTime.
			NTSC_FRAME,

			// For constructing one NTSC 60 hz frame of AnimTime. 
			NTSC_30_FRAME,

			// For constructing one PAL 50 hz frame of AnimTime. 
			PAL_FRAME,

			// For constructing one microsecond of AnimTime. 
			ONE_MICROSECOND,

			// For constructing one millisecond of AnimTime. 
			ONE_MILLISECOND,

			// For constructing one second of AnimTime. 
			ONE_SECOND,

			// For constructing one minute of AnimTime. 
			ONE_MINUTE,

			// For constructing one hour of AnimTime. 
			ONE_HOUR,

			// For constructing the most _negative_ AnimTime that can be represented.
			MIN,

			// For constructing the most positive AnimTime that can be represented.
			MAX,

			// insure the enum is size int
			DWORD = 0x7FFFFFFF
		};

	public:

		//-----------------------------------------------------------------
		// Friends
		//-----------------------------------------------------------------

		/*
		* return: The product of the specified float and AnimTime.
		*
		* Example:
		*    const AnimTime halfSecond = 0.5f * AnimTime(ONE_SECOND);
		*/
		friend const AnimTime operator*(const float, const AnimTime &);

		/*
		* return: The product of the specified integer and AnimTime.
		*
		* Example:
		*    const AnimTime fourMinutes = 4 * AnimTime(ONE_MINUTE);
		*/
		friend const AnimTime operator*(const int, const AnimTime &);

		//-----------------------------------------------------------------
		// Constructors / Destructors / Assignment
		//-----------------------------------------------------------------

		// Creates a AnimTime equal to AnimTime(ZERO). 
		inline AnimTime();

		/*
		* Creates the specific amount of AnimTime.
		*
		* Example:
		*    const AnimTime oneSecond( ONE_SECOND );
		*/
		explicit AnimTime(const Duration);

		// Creates a copy of the specified AnimTime. 
		inline AnimTime(const AnimTime &);

		/*
		* Assigns the right-hand side to this AnimTime.
		* return: this AnimTime.
		*
		*  Example:
		*    const AnimTime tickDuration = AnimTime(NTSC_DURATION);
		*/
		inline AnimTime &operator=(const AnimTime &rhs);

		// destrutor
		~AnimTime()
		{ }

		//-----------------------------------------------------------------
		// Comparisons
		//-----------------------------------------------------------------

		/*
		* return: true if this AnimTime is equal to the right-hand side.
		*
		*  Example:
		*    assert( AnimTime(ONE_HOUR)   == 60 * AnimTime(ONE_MINUTE) );
		*    assert( AnimTime(ONE_MINUTE) == 60 * AnimTime(ONE_SECOND) );
		*/
		bool operator==(const AnimTime &rhs) const;

		/*
		* return: true if this AnimTime is not equal to the right-hand side.
		*/
		bool operator!=(const AnimTime &rhs) const;

		/*
		* return: true if this AnimTime is less than the right-hand side.
		*
		* Example:
		*    bool couldBeOttosMileAnimTim( const AnimTime t )
		*    {
		*       return( t < 4 * AnimTime(ONE_MINUTE) );
		*    }
		*/
		bool operator<(const AnimTime &rhs) const;

		/*
		* return: true if this AnimTime is less or equal to the right-hand side.
		*/
		bool operator<=(const AnimTime &rhs) const;

		/*
		* return: true if this AnimTime is greater than the right-hand side.
		*/
		bool operator>(const AnimTime &rhs) const;

		/*
		* return: true if this AnimTime is greather than or equal to
		*  the right-hand side.
		*/
		bool operator>=(const AnimTime &rhs) const;

		//-----------------------------------------------------------------
		// Negation / Addition / Subtraction
		//-----------------------------------------------------------------

		/*
		* return: A negative copy of this AnimTime.
		*
		* Example:
		*    void neverFail( const AnimTime t )
		*    {
		*       assert( t + (-t) == AnimTime(ZERO) );
		*    }
		*/
		const AnimTime operator-() const;

		/*
		* return: The sum of this AnimTime and the right-hand side.
		*
		* Example:
		*    void neverFail( const AnimTime t )
		*    {
		*       assert( t + t == 2 * t );
		*    }
		*/
		const AnimTime operator+(const AnimTime &rhs) const;

		/*
		* return: The difference between this AnimTime and the right-hand side.
		*
		* Example:
		*    void neverFail( const AnimTime t )
		*    {
		*       assert( t - t == AnimTime(ZERO) );
		*    }
		*/
		const AnimTime operator-(const AnimTime &rhs) const;

		/*
		* Adds the right-hand side to this AnimTime.
		*
		* return: This AnimTime.
		*
		*  Example:
		*    void zeroOutAnimTim( AnimTime& t )
		*    {
		*       t += (-t);
		*       assert( AnimTime(ZERO) == t );
		*    }
		*/
		AnimTime &operator+=(const AnimTime &rhs);

		/*
		* Subtracts the right-hand side from this AnimTime.
		*
		* return: This AnimTime.
		*
		*  Example:
		*    void zeroOutAnimTim( AnimTime& t )
		*    {
		*       t -= t;
		*       assert( AnimTime(ZERO) == t );
		*    }
		*/
		AnimTime &operator-=(const AnimTime &rhs);

		//-----------------------------------------------------------------
		// Multiplication
		//-----------------------------------------------------------------

		/*
		* return: The product of this AnimTime and the specified float.
		*
		* Example:
		*    const AnimTime halfSecond = AnimTime(ONE_SECOND) * 0.5f;
		*/
		const AnimTime operator*(const float) const;

		/*
		* return: The product of this AnimTime and the specified integer.
		*
		*  Example:
		*    const AnimTime fourMinutes = AnimTime(ONE_MINUTE) * 4;
		*/
		const AnimTime operator*(const int) const;

		/*
		* Multiplies this AnimTime by the specified float.
		*
		* return: This AnimTime.
		*
		* Example:
		*    void decayAnimTim( AnimTime& t )
		*    {
		*       t *= 0.95f;
		*    }
		*/
		AnimTime &operator*=(const float);

		/*
		* Multiplies this AnimTime by the specified integer.
		*
		* return: This AnimTime.
		*
		* Example:
		*    void doubleAnimTim( AnimTime& t )
		*    {
		*       t *= 2;
		*    }
		*/
		AnimTime &operator*=(const int);

		//-----------------------------------------------------------------
		// name: Division
		//-----------------------------------------------------------------

		/*
		* return: The ratio of this AnimTime divided by the denominator AnimTime.
		*
		* note: Dividing by AnimTime::ZERO is undefined.
		*/
		float operator/(const AnimTime &denominator) const;

		/*
		* return: The result of dividing this AnimTime and by the
		* specified float.
		*
		* note: Dividing by zero is undefined.
		*/
		const AnimTime operator/(const float denominator) const;

		/*
		* return: The result of dividing this AnimTime and by the
		* specified integer.
		*
		* note: Dividing by zero is undefined.
		*/
		const AnimTime operator/(const int denominator) const;

		/*
		* Divides this AnimTime by the specified float.
		*
		* return: This AnimTime.
		*
		* note: Dividing by zero is undefined.
		*/
		AnimTime &operator/=(const float);

		/*
		* Divides this AnimTime by the specified integer.
		*
		* return: This AnimTime.
		*
		* note: Dividing by zero is undefined.
		*/
		AnimTime &operator/=(const int);

		//-----------------------------------------------------------------
		// Quotient / Remainder
		//-----------------------------------------------------------------

		/*
		* return: The <i>truncated</i>, integral division of the
		* numerator and denominator.  In other words,
		* the result of the division is rounded down to the nearest
		* integer.
		*
		* note: Dividing by AnimTime(ZERO) is undefined.
		*/
		static int 	Quotient(const AnimTime &numerator, const AnimTime &denominator);

		/*
		* return: The remainder of dividing the numerator into the
		* denominator.  This is the equivalent of a modulus operation.
		*
		* note: Dividing by AnimTime(ZERO) is undefined.
		*/
		static const AnimTime 	Remainder(const AnimTime &numerator, const AnimTime &denominator);

	private:
		// types
		typedef long long Representation;

		// constructor
		explicit AnimTime(const Representation);

		// data
		Representation privRawAnimTime;
	};


	//---------------------------------------------------------------------------
	// INLINED IMPLEMENTATION:
	//---------------------------------------------------------------------------
	AnimTime::AnimTime() : privRawAnimTime(0)
	{
	}


	AnimTime::AnimTime(const AnimTime &rhs) : privRawAnimTime(rhs.privRawAnimTime)
	{
	}


	AnimTime &AnimTime::operator=(const AnimTime &rhs)
	{
		if (this != &rhs)
		{
			this->privRawAnimTime = rhs.privRawAnimTime;
		}

		return(*this);
	}


}

#endif  

// ---  End of File ---
