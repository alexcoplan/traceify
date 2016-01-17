#include "light.hpp"

Light::Light(const vec3& position, RGBVec col)
	: pos(position), colour(col) {}

vec3 Light::lVectorFromPoint(const vec3 &p) {
	return (pos - p).normalised();
};
