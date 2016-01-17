/* world.hpp
 *
 * includes the definition of the world
 * and all the world-like things (scene objects, for example)
 * */

#include <vector>
#include <string>

#include "ray.hpp"
#include "viewport.hpp"
#include "colour.hpp"
#include "light.hpp"

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
	Sphere(vec3 c, double r, RGBVec k_d);
	Sphere(const Sphere &s);
	IntersectionResult intersects(const Ray &r);
	SceneObject *makeCopy();
	SceneObject *makeCopy() const;
	vec3 surfaceNormal(const vec3 &point);
	vec3 surfaceNormal(const vec3 &point) const;
};

class World {
public:
	Viewport viewport;
	RGBVec bg_colour;
	Light light;

	~World();
	World(Viewport, RGBVec bg_colour, Light light); 
	void addObject(const SceneObject&);
	RGBColour traceRayAt(int i, int j);

private:
	std::vector<SceneObject*> scenery;
	const vec3 uAxis;
	const vec3 vAxis;
	const vec3 wAxis;
	const vec3 cameraPosition;
};
