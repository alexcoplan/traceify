#include <iostream>
#include <string>
#include <fstream>

#include "colour.hpp"
#include "vec3.hpp"
#include "world.hpp"
#include "image.hpp"

#define IMG_SIZE 1000
#define CAMERA_WIDTH 0.1
#define VIEWING_DISTANCE 0.25
#define SPHERE_RADIUS 0.4


void trace_dem_rays()
{
	const double sphere_distance = 30.0;

	World world(
			Viewport(IMG_SIZE, CAMERA_WIDTH, VIEWING_DISTANCE), 	// camera/viewport setup
			RGBVec()					    	// background colour (init black)		
	);

	Light l1(vec3(-3.0,2.0,sphere_distance-10.0), RGBVec(0.5,0.5,0.5));
	Light l2(vec3(1.0,3.0,6.0), RGBVec(0.5,0.5,0.5));
	Light backLight(vec3(1.0,10.0,30.0), RGBVec(0.8,0.8,0.8));
	world.addLight(l1);
	world.addLight(l2);
	world.addLight(backLight);
	
	Image img(world.viewport.pixelsWide(), world.viewport.pixelsTall());

	// base material for spheres: specularity 2.0, ambient 0.05 
	// this mateiral is green, we should change this for each sphere
	Material sphere_mat(RGBVec(0.0,0.8,0.0), RGBVec(0.2,0.2,0.2), 32.0, 0.05);
	sphere_mat.reflective = false;

	/*
	// green sphere
	vec3 sphere_centre(-2.0,0.0,sphere_distance-1.5);
	Sphere s(sphere_centre, SPHERE_RADIUS, sphere_mat); 
	world.addObject(s);

	// red sphere
	const vec3 s2_centre(2.0,0.0,sphere_distance+2.0);
	sphere_mat.material_colour = RGBVec(1.0,0.0,0.0);
	Sphere s2(s2_centre, SPHERE_RADIUS, sphere_mat);
	//world.addObject(s2);

	// blue sphere
	const vec3 s3_centre(-2.0,-1.0,20.0);
	sphere_mat.material_colour = RGBVec(0.0,0.0,1.0);
	Sphere s3(s3_centre, SPHERE_RADIUS/3.0, sphere_mat);
	//world.addObject(s3);
	*/

	// floor plane
	const vec3 floor_normal(0.0, 1.0, 0.0);
	const double plane_const = 2.0;
	const RGBVec kinda_blue(0.8, 0.8, 1.0);
	const RGBVec plane_reflect(0.4,0.4,0.4);
	Material planeMat(kinda_blue, plane_reflect, 0.0, 0.0, true);
	Plane floor_plane(floor_normal, plane_const, planeMat);
	world.addObject(floor_plane);

	/*
	// back plane
	const vec3 back_normal(0.0, 0.0, -1.0);
	const double back_const = 40.0;
	const RGBVec backing_colour = RGBVec(0.15,0.15,0.2);
	Material backMat(backing_colour, plane_reflect, 0.0, 0.0, false);
	Plane back_plane(back_normal, back_const, backMat);
	world.addObject(back_plane);
	*/

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				double a = (double)i;
				double b = (double)j;
				double c = (double)k;
				vec3 pos(-1.0 + a*2.0, b*2.0 - 1.0, sphere_distance + c*2.0);
				D( std::cerr << "adding sphere @ "; )
				D( pos.debug_print(); )

				const double dc = 1.0/3.0;
				RGBVec col(dc*(a+1.0), dc*(b+1.0), dc*(c+1.0));
				D( std::cerr << "colour: "; )
				D( col.debug_print(); )
				RGBVec spec(0.2, 0.2, 0.2);
				Material mat(col,spec,0.0,0.0,true); 
				Sphere s(pos, 0.2, mat);
				world.addObject(s);
			}
		}
	}

	for (int i = 0; i < IMG_SIZE; i++) {
		for (int j = 0; j < IMG_SIZE; j++) {
			img[i][j] = world.colourForPixelAt(i,j);
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
