#include <iostream>
#include <string>
#include <fstream>

#include "colour.hpp"
#include "vec3.hpp"
#include "world.hpp"
#include "image.hpp"

#define IMG_SIZE 500
#define CAMERA_WIDTH 0.1
#define VIEWING_DISTANCE 0.1
#define SPHERE_RADIUS 0.4


void trace_dem_rays()
{
	const double sphere_distance = 20.0;

	World world(
			Viewport(IMG_SIZE, CAMERA_WIDTH, VIEWING_DISTANCE), 	// camera/viewport setup
			RGBVec(),					    	// background colour (init black)
			Light(vec3(-3.0,-1.0,sphere_distance-10.0), RGBVec(0.9,0.9,0.9))		// set up point light
	);
	
	Image img(world.viewport.pixelsWide(), world.viewport.pixelsTall());
	
	vec3 sphere_centre(0.0,0.0,sphere_distance);
	Sphere s(sphere_centre, SPHERE_RADIUS, RGBVec(0.0,1.0,0.0)); 
	world.addObject(s);

	for (int i = 0; i < IMG_SIZE; i++) {
		for (int j = 0; j < IMG_SIZE; j++) {
			img[i][j] = world.traceRayAt(i,j);
		}
	}

	img.writeToFile("render2.pmm");
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
