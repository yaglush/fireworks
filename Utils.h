#pragma once

#include <random>
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>

namespace Utils
{
	float RndFrom0to1();

	float RndFromTo_F( const float min, const float max );
	unsigned RndFromTo_I( const unsigned min, const unsigned max );

	const float gravity = 98.f;
}