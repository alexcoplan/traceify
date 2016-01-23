#include "world.hpp"
#include "debug.h"

#define REFLECTION_EPS 0.00001
#define MAX_TRACE_DEPTH 3

// World
World::World(Viewport vp, RGBVec bg) :
       	viewport(vp), bg_colour(bg),
	uAxis(1.0,0.0,0.0), // set up camera basis
	vAxis(0.0,1.0,0.0),
	wAxis(0.0,0.0,-1.0),
	cameraPosition(0.0,0.0,-0.1) {}

World::~World() {
	for (size_t i = 0; i < scenery.size(); i++) {
		delete scenery[i];
	}
}

void World::addObject(const SceneObject &s) {
	SceneObject *obj = s.makeCopy();
	scenery.push_back(obj);
}

void World::addLight(const Light &l) {
	lighting.push_back(l);
}

// returns true if under shadow
bool World::traceShadowRay(const Ray &ray) {
	const double shadow_eps = 0.00001;

	for (std::vector<SceneObject*>::iterator it = scenery.begin(); it != scenery.end(); it++) {
		IntersectionResult iResult = (*it)->intersects(ray);
		if (iResult.intersected && iResult.coefficient > shadow_eps) return true;
	}
	return false;
}


RGBVec World::traceRay(const Ray &ray, double t_min, int depth) {
	const SceneObject *closestObject = NULL;
	double t = 0.0; 

	for (std::vector<SceneObject*>::iterator it = scenery.begin();
			it != scenery.end(); it++) {
		SceneObject *scene_obj = *it;
		IntersectionResult iResult = (*scene_obj).intersects(ray);
		if (iResult.intersected && iResult.coefficient > t_min) {
			if (closestObject == NULL) {
				closestObject = scene_obj;
				t = iResult.coefficient;	
			}
			else if (iResult.coefficient < t) {
				closestObject = scene_obj;
				t = iResult.coefficient;
			}
		}	
	}

	if (closestObject == NULL) {
		return bg_colour; 
	}
	
	RGBVec result_vec;

	// compute lighting/shading
	for (std::vector<Light>::iterator lptr = lighting.begin(); lptr != lighting.end(); lptr++) {
		vec3 p = ray.intersectionPoint(t);
		vec3 n = closestObject->surfaceNormal(p);
		vec3 v = (ray.origin - lptr->pos).normalised();
		vec3 l = (lptr->pos - p).normalised();

		Ray shadowRay(p,l);
	
		// if we're not in shadow w.r.t this light
		if (!traceShadowRay(shadowRay))
			result_vec += closestObject->material.shade(*lptr, n, v, l); 


		if (closestObject->material.reflective && depth < MAX_TRACE_DEPTH) {
			// recursively trace reflection rays:
			
			vec3 d = ray.direction;
			Ray reflected(p, d - n.scaled(2 * d.dot(n)));
			RGBVec reflectedColour = traceRay(reflected, REFLECTION_EPS, depth + 1);
			RGBVec k_m = closestObject->material.specular_colour;
			result_vec += reflectedColour.multiplyColour(k_m);
		}

	}

		
	return result_vec; 
}	

RGBColour World::colourForPixelAt(int i, int j) {
	double uValue = viewport.uAmount(i);
	double vValue = viewport.vAmount(j);
	double d = viewport.getViewingDistance();
	vec3 direction = wAxis.scaled(-d) + uAxis.scaled(uValue) + vAxis.scaled(vValue);	

	Ray theRay(cameraPosition, direction);
	
	// begin tracing with t_min = 0, depth = 0
	return  RGBColour(traceRay(theRay, 0.0, 0));
}	
