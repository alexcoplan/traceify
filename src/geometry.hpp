/* geometry.hpp
 *
 * deals with the various kinds of primitive
 * we can intersect with
 *
 * TODO: add triangles, cylinders, etc.
 *
 */

#include <vector>

#include "ray.hpp"
#include "material.hpp"

struct BoundingBox {
	double x_min;
	double x_max;
	double y_min;
	double y_max;
	double z_min;
	double z_max;

	BoundingBox();
	void swallow(const BoundingBox &b);
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
	virtual bool isCluster() = 0;
	virtual bool isCluster() const = 0;
	virtual BoundingBox getBoundBox() = 0;
	virtual BoundingBox getBoundBox() const = 0;
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

/* Cluster
 *
 * if we have many objects that are close together
 * then we can group them to a cluster
 *
 * this can improve efficiency by first checking if the ray
 * intersects this bounding box, and only then checking if it intersects
 * each of the individual objets in the cluster
 *
 * TODO: add hierarchical bounding boxes
 */
class Cluster : public SceneObject {
private:
	BoundingBox bb;

public:
	std::vector<SceneObject *> boundedObjects; 
	
	// overriden methods:
	bool isCluster();
	bool isCluster() const;
	IntersectionResult intersects(const Ray &r);
	IntersectionResult intersects(const Ray &r) const;
	SceneObject *makeCopy();
	SceneObject *makeCopy() const;
	std::string tag();
	std::string tag() const;
	BoundingBox getBoundBox();
	BoundingBox getBoundBox() const;

	// specific methods
	Cluster();
	Cluster(const Cluster &);
	void addObject(const SceneObject &);
	~Cluster();
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
	bool isCluster();
	bool isCluster() const; 
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
	BoundingBox getBoundBox();
	BoundingBox getBoundBox() const;
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
	BoundingBox getBoundBox();
	BoundingBox getBoundBox() const;
};


