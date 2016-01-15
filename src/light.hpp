#include "colour.hpp"

class Light {
public:
	vec3 pos;
	RGBVec colour;

	Light(const vec3& pos, RGBVec colour);
};
