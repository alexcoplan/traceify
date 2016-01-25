#include <iostream>
#include <string>
#include <fstream>

#include "colour.hpp"
#include "vec3.hpp"
#include "world.hpp"
#include "image.hpp"

#define IMG_WIDTH 1000
#define IMG_HEIGHT 800
#define CAMERA_WIDTH 0.1
#define VIEWING_DISTANCE 0.25
#define SPHERE_RADIUS 0.4

void trace_dem_rays()
{
	const double sphere_distance = 30.0;

	World world(
			Viewport(IMG_WIDTH, IMG_HEIGHT, CAMERA_WIDTH, VIEWING_DISTANCE), 
			vec3(-8.8, 4.5, -0.1), // camera position
			RGBVec()					    
	);

	// Camera Setup
	world.cameraRotateY(0.3); // ~10 degrees to the right
	world.cameraRotateX(-0.08);

	// Lighting Setup
	Light l1(vec3(3.0,5.0,sphere_distance-10.0), RGBVec(0.4,0.4,0.4));
	Light l2(vec3(1.0,5.0,5.0), RGBVec(0.5,0.5,0.5));
	Light top1(vec3(0.0,5.0,sphere_distance), RGBVec(0.2,0.2,0.2));
	Light top2(vec3(0.0,5.0,sphere_distance - 2.0), RGBVec(0.4,0.4,0.4));
	Light back(vec3(0.0,3.0,sphere_distance+20.0), RGBVec(0.4,0.4,0.4));
	
	world.addLight(l1);
	world.addLight(l2);
	world.addLight(top1);
	world.addLight(top2);
	world.addLight(back);
	
	Image img(world.viewport.pixelsWide(), world.viewport.pixelsTall());

	// floor plane
	const vec3 floor_normal(0.0, 1.0, 0.0);
	const double plane_const = 2.0;
	const RGBVec kinda_blue(0.6, 0.6, 0.8);
	const RGBVec plane_reflect(0.1,0.1,0.1);
	Material planeMat(kinda_blue, plane_reflect, 0.0, 0.0, false);
	Plane floor_plane(floor_normal, plane_const, planeMat);
	world.addObject(floor_plane);

	Cluster sphereGroup;
	
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				double a = (double)i;
				double b = (double)j;
				double c = (double)k;
				vec3 pos(-1.0 + a*2.0, b*2.0, sphere_distance + c*2.0);
				D( std::cerr << "adding sphere @ "; )
				D( pos.debug_print(); )

				const double dc = 1.0/3.0;
				RGBVec col(dc*(a+1.0), dc*(b+1.0), dc*(c+1.0));
				D( std::cerr << "colour: "; )
				D( col.debug_print(); )
				RGBVec spec(0.2, 0.2, 0.2);
				Material mat(col,spec,0.0,0.0,true); // relections disabled for now 
				Sphere s(pos, 0.8, mat);
				sphereGroup.addObject(s);
			}
		}
	}

	world.addObject(sphereGroup);

	for (int i = 0; i < world.viewport.pixelsWide(); i++) {
		for (int j = 0; j < world.viewport.pixelsTall(); j++) {
			img[i][j] = world.colourForPixelAt(i,j, ss_on);
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
