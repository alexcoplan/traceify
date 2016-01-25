#include <iostream>
#include <string>
#include <fstream>
#include <ctime>

#include "colour.hpp"
#include "vec3.hpp"
#include "world.hpp"
#include "image.hpp"

#define IMG_WIDTH 1000
#define IMG_HEIGHT 800
#define CAMERA_WIDTH 0.1
#define VIEWING_DISTANCE 0.25
#define SPHERE_RADIUS 0.4


void render_demo(int num_spheres, int ss_level, bool do_shadows, bool do_reflections, bool profiling)
{
	const double sphere_distance = 30.0;

	World world(
			Viewport(profiling ? 500 : IMG_WIDTH, profiling ? 500 : IMG_HEIGHT, CAMERA_WIDTH, VIEWING_DISTANCE), 
			vec3(-8.8, 4.5, -0.1), // camera position
			RGBVec()					    
	);

	world.shadows_enabled = do_shadows;
	world.reflections_enabled = do_reflections;
	world.ss_level = ss_level;

	// Camera Setup
	world.cameraRotateY(0.3); // ~10 degrees to the right
	world.cameraRotateX(-0.08);

	// Lighting Setup
	Light l2(vec3(1.0,5.0,5.0), RGBVec(0.9,0.9,0.9));
	Light top1(vec3(0.0,5.0,sphere_distance), RGBVec(0.5,0.5,0.5));
	Light top2(vec3(-6.0,5.0,sphere_distance - 2.0), RGBVec(0.8,0.8,0.8));
	Light back(vec3(-3.0,6.0,sphere_distance+10.0), RGBVec(0.6,0.6,0.6));
	
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

	int spheres_added = 0;

	for (int i = 0; i < 3 && spheres_added < num_spheres; i++) {
		for (int j = 0; j < 3 && spheres_added < num_spheres; j++) {
			for (int k = 0; k < 3 && spheres_added < num_spheres; k++) {
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
				spheres_added++;
			}
		}
	}

	world.addObject(sphereGroup);

	for (int i = 0; i < world.viewport.pixelsWide(); i++) {
		for (int j = 0; j < world.viewport.pixelsTall(); j++) {
			img[i][j] = world.colourForPixelAt(i,j);
		}
	}


	if (profiling) {
		std::string n_str = std::to_string(num_spheres); 
		std::string ss_str = "ss:" + std::to_string(ss_level);
		std::string s_str = do_shadows ? "s+" : "s-";
		std::string r_str = do_reflections ? "r+" : "r-";
		img.writeToFile("demo_n" + n_str + "_" + ss_str + "_" + s_str + "_" + r_str  + ".ppm");	
	}
	else {
		world.renderStats.summarise();
		img.writeToFile("render.ppm");
	}
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

void profile_with_params(int ss, bool shadows, bool reflections)
{
	for (int i = 1; i <= 27; i++) {
		std::cout << i << "\t";

		const clock_t begin_t = clock();
		render_demo(i, ss, shadows, reflections, true);
		std::cout << float( clock() - begin_t )/CLOCKS_PER_SEC << std::endl;
	}	

}

void rt_profiler() 
{
	std::cout << "---> Welcome to the traceify profiler <---" << std::endl << std::endl;

	for (int ss = 1; ss <= 2; ss++) {
		if (ss == 1)
			std::cout << "First we will profile without super-sampling: " << std::endl << std::endl;
		else 
			std::cout << "Now with super-sampling: " << std::endl << std::endl;

		std::cout << "No shadows and no reflection:" << std::endl;
		profile_with_params(ss, false, false);	
		std::cout << std::endl;

		std::cout << "Now shadows but no reflection:" << std::endl;
		profile_with_params(ss, true, false);
		std::cout << std::endl;

		std::cout << "Now shadows and reflection:" << std::endl;
		profile_with_params(ss, true, true);
		std::cout << std::endl << std::endl << "-------" << std::endl << std::endl;
	}

}

int main()
{
	render_demo(27, 4, true, true, false);

	return 0;
}
