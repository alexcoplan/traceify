/* world.hpp
 *
 * includes the definition of the world
 * and all the world-like things (scene objects, for example)
 * */

#include <vector>
#include <string>
#include <cmath>

#include "ray.hpp"
#include "viewport.hpp"
#include "colour.hpp"
#include "light.hpp"
#include "geometry.hpp"
#include "debug.h"

enum SuperSamplingMode { ss_off, ss_on, ss_adaptive }; 

struct RenderStats {
	int ss_x4;
	int ss_x16;
	int ss_x64;

	RenderStats();
	void summarise();
};

class World {
public:
	Viewport viewport;
	RGBVec bg_colour;
	RenderStats renderStats;
	bool shadows_enabled;
	bool reflections_enabled;
	int ss_level;
	int ss_mode;

	~World();
	World(Viewport, const vec3 &cameraPos, const RGBVec &bg_colour); 
	void addObject(const SceneObject&);
	void addLight(const Light&);
	IntersectionDatum testIntersection(const Ray &r, double t_min, std::vector<SceneObject*> &objSpace);
	RGBVec traceRay(const Ray &r, double t_min, int depth);
	bool traceShadowRay(const Ray &r, std::vector<SceneObject*> &objspace);
	RGBColour colourForPixelAt(int i, int j);

	void cameraRotateY(double theta);
	void cameraRotateX(double theta);

private:
	std::vector<SceneObject*> scenery;
	std::vector<Light> lighting;
	vec3 uAxis;
	vec3 vAxis;
	vec3 wAxis;
	vec3 cameraPosition;
};
