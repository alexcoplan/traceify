#include "world.hpp"
#include "debug.h"

#define REFLECTION_EPS 	0.00001
#define SHADOW_EPS 	0.00001
#define MAX_TRACE_DEPTH 3

RenderStats::RenderStats() :
	ss_x4(0), ss_x16(0), ss_x64(0) {}

void RenderStats::summarise() {
	std::cout << "--- traceify rendering statistics ---" << std::endl << std::endl;
	std::cout << "--> super-sampling:" << std::endl;
	std::cout << "\tpixels rendered @ x4  : " << ss_x4 << std::endl;
	std::cout << "\tpixels rendered @ x16 : " << ss_x16 << std::endl;
	std::cout << "\tpixels rendered @ x64 : " << ss_x64 << std::endl;	
}

// World
World::World(Viewport vp, const vec3 &camPos, const RGBVec &bg) :
       	viewport(vp), bg_colour(bg),
	shadows_enabled(true),
	reflections_enabled(true), 
	ss_level(2),
	ss_mode(ss_adaptive),
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
	for (std::vector<SceneObject*>::iterator it = objSpace.begin(); it != objSpace.end(); it++) {
		SceneObject *obj = *it;
		IntersectionResult iResult = obj->intersects(ray);
		if (iResult.intersected) {
			if (!obj->isCluster()) {
				if (iResult.coefficient > SHADOW_EPS)
					return true;
			}
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
					if (ir.coefficient > t_min && (closestObject == NULL || ir.coefficient < t)) {
						closestObject = ir.intersectedObj;
						t = ir.coefficient;
					}
				}
			}	
			else if (iResult.coefficient > t_min && (closestObject == NULL || iResult.coefficient < t)) {
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
		if (!shadows_enabled || !traceShadowRay(shadowRay, scenery)) {
			result_vec += obj->material.shade(*lptr, n, v, l); 
		}

		if (reflections_enabled && obj->material.reflective && depth < MAX_TRACE_DEPTH) {
			// recursively trace reflection rays:
			
			vec3 d = ray.direction;
			Ray reflected(p, d - n.scaled(2 * d.dot(n)));
			RGBVec reflectedColour = traceRay(reflected, REFLECTION_EPS, depth + 1);
			if (reflectedColour.r() == 0.0 && reflectedColour.g() == 0.0 && reflectedColour.b() == 0.0) continue;
			RGBVec k_m = obj->material.specular_colour;
			result_vec += reflectedColour.multiplyColour(k_m);
		}

	}
	
	return result_vec; 
}	

// this is not optimal but we don't care
// for tiny n
int int_pow(int x, int n) {
	if (n == 0) return 1;
	return x * int_pow(x, n-1);
}

// TODO: implement adaptive supersampling
RGBColour World::colourForPixelAt(int i, int j) {
	double d = viewport.getViewingDistance();

	if (ss_level == 1) {
		// no super-sampling
		double uValue = viewport.uAmount(i, 1, 0, false);
		double vValue = viewport.vAmount(j, 1, 0, false);
		vec3 direction = wAxis.scaled(-d) + uAxis.scaled(uValue) + vAxis.scaled(vValue);
		Ray theRay(cameraPosition, direction);

		return RGBColour(traceRay(theRay, 0.0, 0));
	}

	// 0.01 => x16, 1.2 => x64
	double thresholds[2] = {0.01, 1.2}; 


	double var = 0.0;
	int lvl_log = 2;

	RGBVec pixelColour;

	while (lvl_log <= ss_level) {
		pixelColour = RGBVec(0.0,0.0,0.0);

		int lvl = int_pow(2, lvl_log - 1); 

		double scale_factor = 1.0 / static_cast<double>(lvl*lvl);

		vec3 sum_x_sq(0.0,0.0,0.0);

		for (int a = 0; a < lvl; a++) {
			for (int b = 0; b < lvl; b++) {
				double uValue = viewport.uAmount(i, ss_level, a, lvl_log > 2);
				double vValue = viewport.vAmount(j, ss_level, b, lvl_log > 2);
				vec3 direction = wAxis.scaled(-d) + uAxis.scaled(uValue) + vAxis.scaled(vValue);	
				Ray theRay(cameraPosition, direction);
				RGBVec sample = traceRay(theRay, 0.0, 0).scaled(scale_factor);
				pixelColour += sample;
				if (ss_level > 2) sum_x_sq += sample.getVector().pointwise(sample.getVector());
			}
		}		

		if (lvl_log == 2 && ss_level > 2) {
			vec3 varvec = sum_x_sq.scaled(scale_factor) - pixelColour.getVector().pointwise(pixelColour.getVector());
			var = varvec.magnitude();
			if (i % 100 == 0 && j % 100 == 0) std::cout << "var: " << var << std::endl;
		}

		if (var < thresholds[lvl_log - 2] || lvl_log == ss_level) break;
		lvl_log++;
	}

	if (lvl_log == 2) renderStats.ss_x4++;
       	else if (lvl_log == 3) renderStats.ss_x16++;
	else if (lvl_log == 4) renderStats.ss_x64++;

	return RGBColour(pixelColour);
}	
