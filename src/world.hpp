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
#include "geometry.hpp"

class World {
public:
	Viewport viewport;
	RGBVec bg_colour;

	~World();
	World(Viewport, RGBVec bg_colour); 
	void addObject(const SceneObject&);
	void addLight(const Light&);
	RGBVec traceRay(const Ray &r);
	bool traceShadowRay(const Ray &r);
	RGBColour colourForPixelAt(int i, int j);

private:
	std::vector<SceneObject*> scenery;
	std::vector<Light> lighting;
	const vec3 uAxis;
	const vec3 vAxis;
	const vec3 wAxis;
	const vec3 cameraPosition;
};
