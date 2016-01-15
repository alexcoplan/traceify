#include "ray.hpp"

IntersectionResult::IntersectionResult() : intersected(false) {} // default to false
IntersectionResult::IntersectionResult(double t) : intersected(true), coefficient(t) {}

Ray::Ray(const vec3& o, const vec3& d) : origin(o), direction(d) {}
