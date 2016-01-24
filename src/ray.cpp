#include "ray.hpp"

IntersectionResult::IntersectionResult() : intersected(false) {} // default to false
IntersectionResult::IntersectionResult(double t) : intersected(true), coefficient(t) {}
IntersectionResult::IntersectionResult(const IntersectionResult &ir) :
	intersected(ir.intersected), coefficient(ir.coefficient) {}

Ray::Ray(const vec3& o, const vec3& d) : origin(o), direction(d) {}

vec3 Ray::intersectionPoint(double t) {
	return origin + direction.scaled(t);
}

vec3 Ray::intersectionPoint(double t) const {
	return origin + direction.scaled(t);
}
