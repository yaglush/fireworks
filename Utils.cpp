#include "Utils.h"

using namespace Utils;

float Utils::RndFrom0to1()
{
	return (float)std::rand() / (float)RAND_MAX;
}

float Utils::RndFromTo_F( const float min, const float max )
{
	return min + RndFrom0to1() * (max - min);
}

unsigned Utils::RndFromTo_I( const unsigned min, const unsigned max )
{
	return min + (unsigned)std::rand() % (max - min + 1);
}
