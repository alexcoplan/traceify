#include "colour.hpp"

#ifndef LIGHT_HEADER_WARRIOR
#define LIGHT_HEADER_WARRIOR

// Light
//
// Point light representation
class Light {
public:
	vec3 pos;
	RGBVec colour;

	Light(const vec3& pos, RGBVec colour);
	vec3 lVectorFromPoint(const vec3 &p);
};

#endif
