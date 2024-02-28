#include <cmath>

#include "VitalizeMath.h"
#include "easing.h"

float easing::easeOutSine( float t ) 
{
	return sin( PI * t * 0.5f );
}

float easing::easeInOutSine( float t ) 
{
	return (sin(PI * t - PI * 0.5f) + 1.0f) * 0.5f;
}

float easing::easeInCubic( float t ) 
{
    return t * t * t;
}

float easing::easeOutCubic( float t ) 
{
    return 1.0f - (float)std::pow(1.0f - t, 3);
}
