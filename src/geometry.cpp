#include "geometry.hpp"

GeometryException::GeometryException(std::string msg) : std::runtime_error(msg) {}

IntersectionDatum::IntersectionDatum() : IntersectionResult(), intersectedObj(NULL) {}
IntersectionDatum::IntersectionDatum(double t, SceneObject *objPtr) : IntersectionResult(t), intersectedObj(objPtr) {}

/* BoundingBox implementation
 *
 * BoundingBox is a simple construct that
 * does what it says on the tin
 *
 * used by Cluster objects */
BoundingBox::BoundingBox() 
	: x_min(0.0), x_max(0.0), y_min(0.0), y_max(0.0), z_min(0.0), z_max(0.0) {}

void BoundingBox::swallow(const BoundingBox &b) {
	if (b.x_min < x_min) x_min = b.x_min;
	if (b.x_max > x_max) x_max = b.x_max;
	if (b.y_min < y_min) y_min = b.y_min;
	if (b.y_max > y_max) y_max = b.y_max;
	if (b.z_min < z_min) z_min = b.z_min;
	if (b.z_max > z_max) z_max = b.z_max;
}

/* SceneObject implementation
 *
 * SceneObject is an abstract class which the actual geometric 
 * primitives (spheres, planes, etc.) implement.
 */

// SceneObject virtual destructor definition
SceneObject::~SceneObject() {}

// Used to contruct the portion of the subclassed
// objects which is a ShadableObject
ShadableObject::ShadableObject(Material mat) : material(mat) {}

bool ShadableObject::isCluster() 	{ return false; }
bool ShadableObject::isCluster() const 	{ return false; }

// note that the `makeCopy` method is necessary to allow us
// to make a heap-allocated copy of a SceneObject
// without knowing its type at compile-time.
// since this method is virtual, it achieves that for us:
SceneObject *Sphere::makeCopy() 	{ return new Sphere(*this); }
SceneObject *Sphere::makeCopy() const 	{ return new Sphere(*this); }

std::string SceneObject::tag() 	{ return "SceneObject"; }
std::string Sphere::tag() 	{ return "Sphere"; }
std::string Plane::tag() 	{ return "Plane"; }
std::string Cluster::tag() 	{ return "Cluster"; }

std::string SceneObject::tag() 	const { return "SceneObject"; }
std::string Sphere::tag() 	const { return "Sphere"; }
std::string Plane::tag() 	const { return "Plane"; }
std::string Cluster::tag() 	const { return "Cluster"; }

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

BoundingBox Sphere::getBoundBox() const {
	BoundingBox bb;

	bb.x_min = centre.x() - radius;
	bb.x_max = centre.x() + radius;
	bb.y_min = centre.y() - radius;
	bb.y_max = centre.y() + radius;
	bb.z_min = centre.z() - radius;
	bb.z_max = centre.z() + radius;

	return bb;
}

BoundingBox Sphere::getBoundBox() {
	return const_cast<const Sphere *>(this)->getBoundBox();
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
	if (t > 0) return IntersectionResult(t);
	
	return IntersectionResult(); // do not count negative intersection
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

BoundingBox Plane::getBoundBox() { 
	throw GeometryException("Cannot get the bounding box of a plane"); 
}

BoundingBox Plane::getBoundBox() const { 
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

// Cluster:
Cluster::Cluster() {}

Cluster::Cluster(const Cluster &c)
	: bb(c.bb) {
	for (size_t i = 0; i < c.boundedObjects.size(); i++) {
		boundedObjects.push_back(c.boundedObjects[i]->makeCopy());
	}	
}

bool Cluster::isCluster()	{ return true; }
bool Cluster::isCluster() const	{ return true; }

SceneObject *Cluster::makeCopy() {
	return new Cluster(*this);
}

SceneObject *Cluster::makeCopy() const {
	return new Cluster(*this);
}

BoundingBox Cluster::getBoundBox() 	 { return bb; }
BoundingBox Cluster::getBoundBox() const { return bb; }

void Cluster::addObject(const SceneObject &obj) {
	D( std::cerr << "adding sphere to cluster group... " );
	SceneObject *obj_ptr = obj.makeCopy();
	boundedObjects.push_back(obj_ptr);
	bb.swallow(obj_ptr->getBoundBox());
	D( std::cerr << "bounding box now given by: " << std::endl; )
	D( std::cerr << "   x_min: " << bb.x_min << ", x_max: " << bb.x_max; )
	D( std::cerr << ", y_min: " << bb.y_min << ", y_max: " << bb.y_max; )
	D( std::cerr << ", z_min: " << bb.z_min << ", z_max: " << bb.z_max << std::endl; )
}

double max(double a, double b, double c) {
	return a > b ? (c > a ? c : a) : (c > b ? c : b);
}

IntersectionResult Cluster::intersects(const Ray &ray) const {
	vec3 e = ray.origin;
	vec3 d = ray.direction;
	
	double t_x_min;
	double t_x_max;

	double a = 1.0 / d.x();
	if (a >= 0) {
		t_x_min = a*(bb.x_min - e.x());
		t_x_max = a*(bb.x_max - e.x());
	}
	else {
		t_x_min = a*(bb.x_max - e.x());
		t_x_max = a*(bb.x_min - e.x());
	}

	double t_y_min;
	double t_y_max;

	a = 1.0 / d.y();
	if (a >= 0) {
		t_y_min = a*(bb.y_min - e.y());
		t_y_max = a*(bb.y_max - e.y());
	}
	else {
		t_y_min = a*(bb.y_max - e.y());
		t_y_max = a*(bb.y_min - e.y());
	}

	double t_z_min;
	double t_z_max;

	a = 1.0 / d.z();
	if (a >= 0) {
		t_z_min = a*(bb.z_min - e.z());
		t_z_max = a*(bb.z_max - e.z());
	}
	else {
		t_z_min = a*(bb.z_max - e.z());
		t_z_max = a*(bb.z_min - e.z());
	}

	if (t_x_min > t_y_max || t_x_min > t_z_max
	 || t_y_min > t_x_max || t_y_min > t_z_max
	 || t_z_min > t_x_max || t_z_min > t_y_max )
		return IntersectionResult();
	return IntersectionResult(max(t_x_min, t_y_min, t_z_min));
}

IntersectionResult Cluster::intersects(const Ray &ray) {
	return const_cast<const Cluster *>(this)->intersects(ray);
}

Cluster::~Cluster() {
	for (size_t i = 0; i < boundedObjects.size(); i++) {
		delete boundedObjects[i];
	}
}
