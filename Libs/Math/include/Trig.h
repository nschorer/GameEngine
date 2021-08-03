//-----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#ifndef ENGINE_MATH_TRIG_H
#define ENGINE_MATH_TRIG_H

namespace Azul
{
	class Trig
	{
	public:

		// Do your magic here

		static float cos(const float  angle_radians);
		static float sin(const float  angle_radians);
		static float tan(const float  val);
		static float atan(const float val);
		static float atan2(const float x, const float y);
		static float acos(const float val );
		static float asin(const float val );
		static void  cossin(float& cos, float& sin, const float angle_radians);
		static float sqrt(const float val);
		static float rsqrt(const float val);
	};
}

#endif

// ---  End of File ---------------
