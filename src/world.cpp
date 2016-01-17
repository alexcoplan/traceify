#include "world.hpp"
#include "debug.h"

// DEBUG
#include <iostream>

// General intersection stuff
class IntersectionException : public std::runtime_error {
public:
	IntersectionException(std::string msg) : std::runtime_error(msg) {}
};

// SceneObject virtual destructor definition
SceneObject::~SceneObject() {}

// makeCopy method is necessary to allow us
// to make a heap-allocated copy of a SceneObject
// without knowing its type at compile-time
//
// since this method is virtual, it achieves that for us
SceneObject *Sphere::makeCopy() {
	return new Sphere(*this);
}

SceneObject *Sphere::makeCopy() const {
	return new Sphere(*this);
}


// Sphere
Sphere::Sphere(vec3 c, double r, RGBVec k_d) : centre(c), radius(r) {
	diffuse_colour = k_d;
}

// Sphere destructor
Sphere::~Sphere() {}

// copy constructor
Sphere::Sphere(const Sphere &s) : centre(s.centre), radius(s.radius) {
	diffuse_colour = s.diffuse_colour;
}

vec3 Sphere::surfaceNormal(const vec3 &p) {
	// assume p is a point on the surface of the sphere
	return (p - centre).normalised();
}

vec3 Sphere::surfaceNormal(const vec3 &p) const {
	// assume p is a point on the surface of the sphere
	return (p - centre).normalised();
}


IntersectionResult Sphere::intersects(const Ray &ray) {
	vec3 e = ray.origin;
	vec3 d = ray.direction;
	vec3 c = this->centre;
	double r = this->radius;

	double b = d.dot(e-c);
	double four_ac = d.dot(d) * (e - c).dot(e-c) - r*r;
	double discriminant = b*b - four_ac;
	if (discriminant < 0) return IntersectionResult(); // no intersection

	// just take - of +/-, since we want closest point of intersection
	double t = (-b - sqrt(discriminant))/(d.dot(d));
	if (t > 0) return IntersectionResult(t);
	
	t = (-b + sqrt(discriminant))/(d.dot(d));
	if (t > 0) return IntersectionResult(t);

	throw IntersectionException("Negative intersection coefficient");
}

// World
World::World(Viewport vp, RGBVec bg, Light the_light) :
       	viewport(vp), bg_colour(bg), light(the_light),
	uAxis(1.0,0.0,0.0), // set up camera basis
	vAxis(0.0,1.0,0.0),
	wAxis(0.0,0.0,-1.0),
	cameraPosition(0.0,0.0,-0.1) {}

World::~World() {
	for (size_t i = 0; i < scenery.size(); i++) {
		delete scenery[i];
	}
}

void World::addObject(SceneObject const &s) {
	SceneObject *obj = s.makeCopy();
	scenery.push_back(obj);
}

RGBColour World::traceRayAt(int i, int j) {
	double uValue = viewport.uAmount(i);
	double vValue = viewport.vAmount(j);
	double d = viewport.getViewingDistance();
	vec3 direction = wAxis.scaled(-d) + uAxis.scaled(uValue) + vAxis.scaled(vValue);	
	/*
	D(
		if (i % 50 == 0 && j % 50 == 0) {
			std::cout << "Casting ray (" << i << "," << j << ") " << "in direction: ";
			direction.debug_print();
		}
	)*/

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
				intersection.coefficient = t;
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

	vec3 intersectionPoint = ray.intersectionPoint(t);
	double alignment = closestObject->surfaceNormal(intersectionPoint).dot(light.lVectorFromPoint(intersectionPoint));
	double coefficient = alignment > 0.0 ? alignment : 0.0;

	RGBVec result_vec(obj_colour.multiplyColour(light.colour).scaled(coefficient)); 

	D(
		if (i % 50 == 0 && j % 50 == 0) {
			std::cout << "Colour = ";
			result_vec.debug_print();
			std::cout << std::endl;
		}
	)
	
	return RGBColour(result_vec);
}	
