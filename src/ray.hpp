#include "vec3.hpp"

struct IntersectionResult {
	bool intersected;
	double coefficient;

	IntersectionResult();		// default, false result
	IntersectionResult(double); 	// true result
};

struct Ray {
	const vec3 origin;
	const vec3 direction;

	Ray(const vec3& origin, const vec3& direction);
};
