/* geometry.hpp
 *
 * deals with the various kinds of primitive
 * we can intersect with
 *
 * TODO: add triangles, cylinders, etc.
 *
 */

#ifndef GEOM_HGUARD
#define GEOM_HGUARD

#define X_AXIS 	0
#define Y_AXIS 	1
#define Z_AXIS 	2
#define BBOX_EPS 0.00001	

#include <vector>

#include "ray.hpp"
#include "material.hpp"

struct BBox {
	static const double inf; // set to IEEE FP infinity (for convenience)
	double min[3];
	double max[3];

	BBox();
	BBox(double initSize); // origin-centered cube with side length `initSize`
	void swallow(const BBox &);
	void operator=(const BBox &);
	vec3 getMidpoint();
	double midpointForAxis(int axis);
};

/* SceneObject
 *
 * Abstract class from which all objects in the scene inherit */
class SceneObject {
public:
	virtual IntersectionResult intersects(const Ray &r) = 0;
	virtual IntersectionResult intersects(const Ray &r) const = 0;
	virtual SceneObject *makeCopy() = 0;
	virtual SceneObject *makeCopy() const = 0;
	virtual std::string tag(); // for debugging
	virtual std::string tag() const;
	virtual BBox getBBox() = 0;
	virtual BBox getBBox() const = 0;
	virtual ~SceneObject();
};

struct IntersectionDatum : public IntersectionResult {
	SceneObject *intersectedObj;
	IntersectionDatum();
	IntersectionDatum(double t, SceneObject *obj);
};

struct GeometryException : public std::runtime_error {
	GeometryException(std::string msg);
};


/* ShadableObject
 *
 * Anything that we want to actually render
 * should inherit from this */
class ShadableObject : public SceneObject {
public:
	Material material;
	ShadableObject(Material mat);
	virtual vec3 surfaceNormal(const vec3 &point) = 0;
	virtual vec3 surfaceNormal(const vec3 &point) const = 0;
};

class Sphere : public ShadableObject {
private:
	const vec3 centre;
	const double radius;

public:
	~Sphere();
	Sphere(const vec3 &c, double r, const Material &mat);
	Sphere(const Sphere &s);
	IntersectionResult intersects(const Ray &r);
	IntersectionResult intersects(const Ray &r) const;
	SceneObject *makeCopy();
	SceneObject *makeCopy() const;
	vec3 surfaceNormal(const vec3 &point);
	vec3 surfaceNormal(const vec3 &point) const;
	std::string tag();
	std::string tag() const;
	BBox getBBox();
	BBox getBBox() const;
};

class Plane : public ShadableObject {
private:
	const vec3 normal;
	const double k;

public:
	~Plane();
	Plane(const vec3 &n, double k, const Material &mat);
	Plane(const Plane &p);
	IntersectionResult intersects(const Ray &r);
	IntersectionResult intersects(const Ray &r) const;
	SceneObject *makeCopy();
	SceneObject *makeCopy() const;
	vec3 surfaceNormal(); // since a plane has one normal everywhere
	vec3 surfaceNormal() const;
	vec3 surfaceNormal(const vec3&); // still need to override the pure virtuals though
	vec3 surfaceNormal(const vec3&) const;
	std::string tag();
	std::string tag() const;
	BBox getBBox();
	BBox getBBox() const;
};

#endif
