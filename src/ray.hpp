#include "vec3.hpp"

#ifndef RAY_HEADER_WARRIOR
#define RAY_HEADER_WARRIOR

struct IntersectionResult {
	bool intersected;
	double coefficient;

	IntersectionResult();		// default, false result
	IntersectionResult(double); 	// true result
	IntersectionResult(const IntersectionResult &);
};

struct Ray {
	vec3 origin;
	vec3 direction;

	// t is the coefficient along the ray's line at
	// which the intersection occurred
	vec3 intersectionPoint(double t);
	vec3 intersectionPoint(double t) const;

	Ray(const vec3& origin, const vec3& direction);
};

#endif
