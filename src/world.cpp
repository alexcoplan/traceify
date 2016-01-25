#include "world.hpp"
#include "debug.h"

#define REFLECTION_EPS 0.00001
#define MAX_TRACE_DEPTH 3

// World
World::World(Viewport vp, const vec3 &camPos, const RGBVec &bg) :
       	viewport(vp), bg_colour(bg),
	uAxis(1.0,0.0,0.0), // set up camera basis
	vAxis(0.0,1.0,0.0),
	wAxis(0.0,0.0,-1.0),
	cameraPosition(camPos) {}

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

// Camera stuff
void World::cameraRotateY(double theta) {
	double sin_theta = sin(theta);
	double cos_theta = cos(theta);
	vec3 uPrime = uAxis.scaled(cos_theta) + wAxis.scaled(sin_theta);
	vec3 wPrime = uAxis.scaled(-sin_theta) + wAxis.scaled(cos_theta);
	uAxis = uPrime;
	wAxis = wPrime;
}

void World::cameraRotateX(double theta) {
	double sin_theta = sin(theta);
	double cos_theta = cos(theta);
	vec3 vPrime = vAxis.scaled(cos_theta) + wAxis.scaled(sin_theta);
	vec3 wPrime = vAxis.scaled(-sin_theta) + wAxis.scaled(cos_theta);
	vAxis = vPrime;
	wAxis = wPrime;
}


// returns true if under shadow
bool World::traceShadowRay(const Ray &ray, std::vector<SceneObject*> &objSpace) {
	const double shadow_eps = 0.00001;

	for (std::vector<SceneObject*>::iterator it = objSpace.begin(); it != objSpace.end(); it++) {
		SceneObject *obj = *it;
		IntersectionResult iResult = obj->intersects(ray);
		if (iResult.intersected && iResult.coefficient > shadow_eps) {
			if (!obj->isCluster())
				return true;
			else {
				Cluster *cluster = static_cast<Cluster*>(obj);
				if (traceShadowRay(ray, cluster->boundedObjects))
					return true;
			}
		}
	}

	return false;
}

IntersectionDatum World::testIntersection(const Ray &ray, double t_min, std::vector<SceneObject*> &objectSpace) {
	SceneObject *closestObject = NULL;
	double t = 0.0;

	for (std::vector<SceneObject*>::iterator it = objectSpace.begin(); it != objectSpace.end(); it++) {
		SceneObject *scene_obj = *it;
		IntersectionResult iResult = scene_obj->intersects(ray);
		if (iResult.intersected) {
			if (scene_obj->isCluster()) {
				Cluster *cluster = static_cast<Cluster*>(scene_obj);
				IntersectionDatum ir = testIntersection(ray, t_min, cluster->boundedObjects);
				if (ir.intersected) {
					if (closestObject == NULL || ir.coefficient < t) {
						closestObject = ir.intersectedObj;
						t = ir.coefficient;
					}
				}
			}	
			else if (closestObject == NULL || iResult.coefficient < t) {
				closestObject = scene_obj;
				t = iResult.coefficient;
			}
		}
	}

	if (closestObject == NULL)
		return IntersectionDatum();
	else
		return IntersectionDatum(t, closestObject);
}


RGBVec World::traceRay(const Ray &ray, double t_min, int depth) {
	IntersectionDatum idat = testIntersection(ray, t_min, scenery);
	
	if (!idat.intersected)
		return bg_colour; 
	
	RGBVec result_vec;

	ShadableObject *obj = static_cast<ShadableObject *>(idat.intersectedObj);
	double t = idat.coefficient;	

	// compute lighting/shading
	for (std::vector<Light>::iterator lptr = lighting.begin(); lptr != lighting.end(); lptr++) {
		vec3 p = ray.intersectionPoint(t);
		vec3 n = obj->surfaceNormal(p);
		vec3 v = (ray.origin - lptr->pos).normalised();
		vec3 l = (lptr->pos - p).normalised();

		Ray shadowRay(p,l);
	
		// if we're not in shadow w.r.t this light
		if (!traceShadowRay(shadowRay, scenery))
			result_vec += obj->material.shade(*lptr, n, v, l); 


		if (obj->material.reflective && depth < MAX_TRACE_DEPTH) {
			// recursively trace reflection rays:
			
			vec3 d = ray.direction;
			Ray reflected(p, d - n.scaled(2 * d.dot(n)));
			RGBVec reflectedColour = traceRay(reflected, REFLECTION_EPS, depth + 1);
			RGBVec k_m = obj->material.specular_colour;
			result_vec += reflectedColour.multiplyColour(k_m);
		}

	}
	
	return result_vec; 
}	

RGBColour World::colourForPixelAt(int i, int j, SuperSamplingMode ss_mode) {
	const int ss_level = (ss_mode == ss_on) ? 4 : 1;
	const double scale_factor = 1.0 / static_cast<double>(ss_level*ss_level);

	double d = viewport.getViewingDistance();

	RGBVec pixelColour;

	for (int a = 0; a < ss_level; a++) {
		for (int b = 0; b < ss_level; b++) {
			double uValue = viewport.uAmount(i, ss_level, a);
			double vValue = viewport.vAmount(j, ss_level, b);
			vec3 direction = wAxis.scaled(-d) + uAxis.scaled(uValue) + vAxis.scaled(vValue);	
			Ray theRay(cameraPosition, direction);
			pixelColour += traceRay(theRay, 0.0, 0).scaled(scale_factor);
		}
	}

	return RGBColour(pixelColour);
}	
