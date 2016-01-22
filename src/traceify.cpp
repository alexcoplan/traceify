#include <iostream>
#include <string>
#include <fstream>

#include "colour.hpp"
#include "vec3.hpp"
#include "world.hpp"
#include "image.hpp"

#define IMG_SIZE 500
#define CAMERA_WIDTH 0.1
#define VIEWING_DISTANCE 0.2
#define SPHERE_RADIUS 0.4


void trace_dem_rays()
{
	const double sphere_distance = 25.0;

	World world(
			Viewport(IMG_SIZE, CAMERA_WIDTH, VIEWING_DISTANCE), 	// camera/viewport setup
			RGBVec()					    	// background colour (init black)		
	);

	Light l1(vec3(-3.0,2.0,sphere_distance-10.0), RGBVec(0.5,0.5,0.5));
	Light l2(vec3(-1.0,3.0,6.0), RGBVec(0.5,0.5,0.5));
	world.addLight(l1);
	world.addLight(l2);
	
	Image img(world.viewport.pixelsWide(), world.viewport.pixelsTall());
	
	// green sphere
	vec3 sphere_centre(-2.0,0.0,sphere_distance-1.5);
	Sphere s(sphere_centre, SPHERE_RADIUS, RGBVec(0.0,1.0,0.0)); 
	world.addObject(s);

	// red sphere
	const vec3 s2_centre(2.0,0.0,sphere_distance+2.0);
	Sphere s2(s2_centre, SPHERE_RADIUS, RGBVec(1.0,0.0,0.0));
	world.addObject(s2);

	const vec3 s3_centre(0.0,-0.6,10.0);
	Sphere s3(s3_centre, SPHERE_RADIUS/3.0, RGBVec(0.1,0.1,1.0));
	world.addObject(s3);

	// add plane
	const vec3 plane_normal(0.0, 1.0, 0.0);
	const double plane_const = 2.0;
	const RGBVec off_white(0.95, 0.95, 0.9);
	Plane plane(plane_normal, plane_const, off_white);
	world.addObject(plane);

	for (int i = 0; i < IMG_SIZE; i++) {
		for (int j = 0; j < IMG_SIZE; j++) {
			img[i][j] = world.traceRayAt(i,j);
		}
	}

	img.writeToFile("render.ppm");
}

void draw_test_image()
{
	Image img(20,20);
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			img[i][j] = RGBColour(255,0,0);
		}
	}
	img.writeToFile("test_file.pmm");
}

int main()
{
	trace_dem_rays();
	
	return 0;
}
