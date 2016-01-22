#include "world.hpp"
#include "debug.h"

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
	const double shadow_eps = 0.0001;

	for (std::vector<SceneObject*>::iterator it = scenery.begin(); it != scenery.end(); it++) {
		IntersectionResult iResult = (*it)->intersects(ray);
		if (iResult.intersected && iResult.coefficient > shadow_eps) return true;
	}
	return false;
}


RGBVec World::traceRay(const Ray &ray) {
	const SceneObject *closestObject = NULL;
	double t = 0.0; 

	for (std::vector<SceneObject*>::iterator it = scenery.begin();
			it != scenery.end(); it++) {
		SceneObject *scene_obj = *it;
		IntersectionResult intersection = (*scene_obj).intersects(ray);
		if (intersection.intersected) {
			if (closestObject == NULL) {
				closestObject = scene_obj;
				t = intersection.coefficient;	
			}
			else if (intersection.coefficient < t) {
				closestObject = scene_obj;
				t = intersection.coefficient;
			}
		}	
	}

	if (closestObject == NULL) {
		return bg_colour; 
	}
	
	RGBVec result_vec;

	for (std::vector<Light>::iterator lptr = lighting.begin(); lptr != lighting.end(); lptr++) {
		vec3 p = ray.intersectionPoint(t);
		vec3 n = closestObject->surfaceNormal(p);
		vec3 v = (ray.origin - lptr->pos).normalised();
		vec3 l = (lptr->pos - p).normalised();

		Ray shadowRay(p,l);
	
		// if we're not in shadow w.r.t this light
		if (!traceShadowRay(shadowRay))
			result_vec += closestObject->material.shade(*lptr, n, v, l); 
	}
	
	return result_vec; 
}	

RGBColour World::colourForPixelAt(int i, int j) {
	double uValue = viewport.uAmount(i);
	double vValue = viewport.vAmount(j);
	double d = viewport.getViewingDistance();
	vec3 direction = wAxis.scaled(-d) + uAxis.scaled(uValue) + vAxis.scaled(vValue);	

	Ray theRay(cameraPosition, direction);
	return  RGBColour(traceRay(theRay));
}	
