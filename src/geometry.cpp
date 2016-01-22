#include "geometry.hpp"

class IntersectionException : public std::runtime_error {
public:
	IntersectionException(std::string msg) : std::runtime_error(msg) {}
};

/* SceneObject implementation
 *
 * SceneObject is an abstract class which the actual geometric 
 * primitives (spheres, planes, etc.) implement.
 */

// SceneObject virtual destructor definition
// TODO: experiment with removing this
// can we make the destructor pure virtual??
SceneObject::~SceneObject() {}

// note that the `makeCopy` method is necessary to allow us
// to make a heap-allocated copy of a SceneObject
// without knowing its type at compile-time.
// since this method is virtual, it achieves that for us:
SceneObject *Sphere::makeCopy() 	{ return new Sphere(*this); }
SceneObject *Sphere::makeCopy() const 	{ return new Sphere(*this); }

/* Sphere implementation */
Sphere::~Sphere() {}

Sphere::Sphere(vec3 c, double r, const RGBVec &k_d) : centre(c), radius(r) {
	diffuse_colour = k_d;
}

Sphere::Sphere(const Sphere &s) : centre(s.centre), radius(s.radius) {
	diffuse_colour = s.diffuse_colour;
}

vec3 Sphere::surfaceNormal(const vec3 &p) {
	// assume p is a point on the surface of the sphere
	return (p - centre).normalised();
}

vec3 Sphere::surfaceNormal(const vec3 &p) const {
	// assume p is a point on the surface of the sphere
	return (p - centre).normalised();
}

// Sphere Intersection
IntersectionResult Sphere::intersects(const Ray &ray) {
	vec3 e = ray.origin;
	vec3 d = ray.direction;
	vec3 c = this->centre;
	double r = this->radius;

	double b = d.dot(e-c);
	double four_ac = d.dot(d) * (e - c).dot(e-c) - r*r;
	double discriminant = b*b - four_ac;
	if (discriminant < 0) return IntersectionResult(); // no intersection

	// just take - of +/-, since we want closest point of intersection
	double t = (-b - sqrt(discriminant))/(d.dot(d));
	if (t > 0) return IntersectionResult(t);
	
	t = (-b + sqrt(discriminant))/(d.dot(d));
	if (t > 0) return IntersectionResult(t);

	throw IntersectionException("Negative intersection coefficient");
}

/* Plane implementation
 *
 * Note that planes are of the form
 * 	r.n + k = 0
 * Where r is the position vector, n is the unit normal of the plane
 * and k is a scalar
 */

// necessary (but boring) memory management stuff
SceneObject *Plane::makeCopy() 		{ return new Plane(*this); }
SceneObject *Plane::makeCopy() const 	{ return new Plane(*this); }

Plane::~Plane() {}

Plane::Plane(vec3 norm, double scalar, const RGBVec &k_d) : normal(norm), k(scalar) {
	diffuse_colour = k_d;
}

Plane::Plane(const Plane &p) : normal(p.normal), k(p.k) {
       	diffuse_colour = p.diffuse_colour;
} 

vec3 Plane::surfaceNormal() 			{ return normal; }
vec3 Plane::surfaceNormal() const 		{ return normal; }
vec3 Plane::surfaceNormal(const vec3&) 		{ return normal; }
vec3 Plane::surfaceNormal(const vec3&) const 	{ return normal; }

// Plane Intersection
IntersectionResult Plane::intersects(const Ray &ray) {
	vec3 e = ray.origin;
	vec3 d = ray.direction;
	vec3 n = this->normal;
	double d_dot_n = d.dot(n);
	if (d_dot_n == 0.0) return IntersectionResult(); // ray skew to or in plane
	double t = -(k + e.dot(n)) / d_dot_n;
	if (t < 0.0) return IntersectionResult(); // behind camera
	return IntersectionResult(t);
}
