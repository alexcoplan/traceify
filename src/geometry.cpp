#include "geometry.hpp"

GeometryException::GeometryException(std::string msg) : std::runtime_error(msg) {}

IntersectionDatum::IntersectionDatum() : IntersectionResult(), intersectedObj(NULL) {}
IntersectionDatum::IntersectionDatum(double t, SceneObject *objPtr) : IntersectionResult(t), intersectedObj(objPtr) {}

/* BBox implementation
 *
 * BBox is a simple bounding box construct.
 *
 * used in the BVH implementation */

const double BBox::inf = std::numeric_limits<double>::infinity();

BBox::BBox() {
	for (int i = 0; i < 2; i++) {
		min[i] = max[i] = 0.0;
	}
}

BBox::BBox(double initSize) {
	for (int i = 0; i < 2; i++) {
		min[i] = -initSize;
		max[i] = initSize;
	}
}

void BBox::swallow(const BBox &b) {
	for (int i = 0; i < 2; i++) {
		if (b.min[i] < min[i]) min[i] = b.min[i];
		if (b.max[i] > max[i]) max[i] = b.max[i];
	}
}

void BBox::operator=(const BBox &b) {
	for (int i = 0; i < 2; i++) {
		min[i] = b.min[i];
		max[i] = b.max[i];
	}
}

double BBox::midpointForAxis(int axis) {
	if (min[axis] == -inf || max[axis] == inf) return 0.0;
	return (min[axis] + max[axis]) / 2.0;
}

vec3 BBox::getMidpoint() {
	return vec3(midpointForAxis(X_AXIS), midpointForAxis(Y_AXIS), midpointForAxis(Z_AXIS));
}

/* SceneObject implementation
 *
 * SceneObject is a high-level abstract class which all the 
 * constructs in the scene (spheres, clusters, planes, etc.) implement.
 */

// need this for the virtual destructor to compile
SceneObject::~SceneObject() {}

// note that the `makeCopy` method is necessary to allow us
// to make a heap-allocated copy of a SceneObject
// without knowing its type at compile-time.
// since this method is virtual, it achieves that for us.
//
// the down-side is that it must be re-implemented for each subclass of SceneObject
// here is the Sphere implementation:
SceneObject *Sphere::makeCopy() 	{ return new Sphere(*this); }
SceneObject *Sphere::makeCopy() const 	{ return new Sphere(*this); }

// tag()s are useful for debugging (e.g. printing what kind of object we're intersecting with)
std::string SceneObject::tag() 	{ return "SceneObject"; }
std::string Sphere::tag() 	{ return "Sphere"; }
std::string Plane::tag() 	{ return "Plane"; }

std::string SceneObject::tag() 	const { return "SceneObject"; }
std::string Sphere::tag() 	const { return "Sphere"; }
std::string Plane::tag() 	const { return "Plane"; }

/* Sphere implementation */
Sphere::~Sphere() {}

Sphere::Sphere(const vec3 &c, double r, const Material &mat) :
	ShadableObject(mat), centre(c), radius(r) {}

Sphere::Sphere(const Sphere &s) :
	ShadableObject(s.material), centre(s.centre), radius(s.radius) {}

vec3 Sphere::surfaceNormal(const vec3 &p) {
	// assume p is a point on the surface of the sphere
	return (p - centre).normalised();
}

vec3 Sphere::surfaceNormal(const vec3 &p) const {
	// assume p is a point on the surface of the sphere
	return (p - centre).normalised();
}

BBox Sphere::getBBox() const {
	BBox bb;

	bb.min[X_AXIS] = centre.x() - radius;
	bb.min[Y_AXIS] = centre.y() - radius;
	bb.min[Z_AXIS] = centre.z() - radius;

	bb.max[X_AXIS] = centre.x() + radius;
	bb.max[Y_AXIS] = centre.y() + radius;
	bb.max[Z_AXIS] = centre.z() + radius;

	return bb;
}

BBox Sphere::getBBox() {
	return const_cast<const Sphere *>(this)->getBBox();
}

// Sphere Intersection
IntersectionResult Sphere::intersects(const Ray &ray) const {
	vec3 e = ray.origin;
	vec3 d = ray.direction;
	vec3 c = this->centre;
	double r = this->radius;

	double b = d.dot(e-c);
	double four_ac = d.dot(d) * ((e - c).dot(e-c) - r*r);
	double discriminant = b*b - four_ac;
	if (discriminant < 0) return IntersectionResult(); // no intersection

	// just take - of +/-, since we want closest point of intersection
	double t = (-b - sqrt(discriminant))/(d.dot(d));
	return IntersectionResult(t);
}

// use the const overloaded version for non-const objects
IntersectionResult Sphere::intersects(const Ray &r) {
	return const_cast<const Sphere*>(this)->intersects(r);
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

Plane::Plane(const vec3 &norm, double scalar, const Material &mat) : 
	ShadableObject(mat), normal(norm), k(scalar) {}

Plane::Plane(const Plane &p) : 
	ShadableObject(p.material), normal(p.normal), k(p.k) {}

vec3 Plane::surfaceNormal() 			{ return normal; }
vec3 Plane::surfaceNormal() const 		{ return normal; }
vec3 Plane::surfaceNormal(const vec3&) 		{ return normal; }
vec3 Plane::surfaceNormal(const vec3&) const 	{ return normal; }

BBox Plane::getBBox() { 
	vec3 pwnorm = normal.pointwiseAbsolute(); // ignore signs in upcoming comparision
	BBox result(BBox::inf); // infinite bbox
	int axisToBound = -1;
	
	if (pwnorm == vec3::iVec) 	axisToBound = X_AXIS;
	else if (pwnorm == vec3::jVec) 	axisToBound = Y_AXIS;
	else if (pwnorm == vec3::kVec) 	axisToBound = Z_AXIS;

	if (axisToBound != -1) {
		result.max[axisToBound] = BBOX_EPS;
		result.min[axisToBound] = -BBOX_EPS;
	}

	return result;
}

BBox Plane::getBBox() const { 
	throw GeometryException("Cannot get the bounding box of a plane"); 
}

// Plane Intersection
IntersectionResult Plane::intersects(const Ray &ray) const {
	vec3 e = ray.origin;
	vec3 d = ray.direction;
	vec3 n = this->normal;
	double d_dot_n = d.dot(n);
	if (d_dot_n == 0.0) return IntersectionResult(); // ray skew to or in plane
	double t = -(k + e.dot(n)) / d_dot_n;
	if (t < 0.0) return IntersectionResult(); // behind camera
	return IntersectionResult(t);
}

IntersectionResult Plane::intersects(const Ray &ray) {
	return const_cast<const Plane *>(this)->intersects(ray);
}



