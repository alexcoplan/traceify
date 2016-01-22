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

RGBColour World::traceRayAt(int i, int j) {
	double uValue = viewport.uAmount(i);
	double vValue = viewport.vAmount(j);
	double d = viewport.getViewingDistance();
	vec3 direction = wAxis.scaled(-d) + uAxis.scaled(uValue) + vAxis.scaled(vValue);	

	Ray ray(cameraPosition, direction); 

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

	D(
		if (i % 50 == 0 && j % 50 == 0 && closestObject != NULL)
			std::cout << "Intersection at (" << i << "," << j << ")!, t = " << t << std::endl;
	)

	if (closestObject == NULL) {
		return RGBColour(bg_colour);
	}
	
	RGBVec obj_colour(closestObject->diffuse_colour);

	RGBVec result_vec;

	for (std::vector<Light>::iterator lptr = lighting.begin(); lptr != lighting.end(); lptr++) {
		vec3 intersectionPoint = ray.intersectionPoint(t);
		double alignment = closestObject->surfaceNormal(intersectionPoint).dot(lptr->lVectorFromPoint(intersectionPoint));
		double coefficient = alignment > 0.0 ? alignment : 0.0;
		result_vec += obj_colour.multiplyColour(lptr->colour).scaled(coefficient); 
	}

	D(
		if (i % 50 == 0 && j % 50 == 0) {
			std::cout << "Colour = ";
			result_vec.debug_print();
			std::cout << std::endl;
		}
	)
	
	return RGBColour(result_vec);
}	
