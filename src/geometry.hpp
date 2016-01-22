/* geometry.hpp
 *
 * deals with the various kinds of primitive
 * we can intersect with
 */

#include "ray.hpp"
#include "colour.hpp"

class SceneObject {
public:
	RGBVec diffuse_colour;
	virtual IntersectionResult intersects(const Ray &r) = 0;
	virtual SceneObject *makeCopy() = 0;
	virtual SceneObject *makeCopy() const = 0;
	virtual vec3 surfaceNormal(const vec3 &point) = 0;
	virtual vec3 surfaceNormal(const vec3 &point) const = 0;
	virtual ~SceneObject();
};

class Sphere : public SceneObject {
private:
	const vec3 centre;
	const double radius;

public:
	~Sphere();
	Sphere(vec3 c, double r, const RGBVec &k_d);
	Sphere(const Sphere &s);
	IntersectionResult intersects(const Ray &r);
	SceneObject *makeCopy();
	SceneObject *makeCopy() const;
	vec3 surfaceNormal(const vec3 &point);
	vec3 surfaceNormal(const vec3 &point) const;
};

class Plane : public SceneObject {
private:
	const vec3 normal;
	const double k;

public:
	~Plane();
	Plane(vec3 n, double k, const RGBVec &k_d);
	Plane(const Plane &p);
	IntersectionResult intersects(const Ray &r);
	SceneObject *makeCopy();
	SceneObject *makeCopy() const;
	vec3 surfaceNormal(); // since a plane has one normal everywhere
	vec3 surfaceNormal() const;
	vec3 surfaceNormal(const vec3&); // still need to override the pure virtuals though
	vec3 surfaceNormal(const vec3&) const;
};

