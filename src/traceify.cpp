#include <iostream>
#include <string>
#include <fstream>

#include "colour.hpp"
#include "vec3.hpp"
#include "world.hpp"

#define IMG_SIZE 500
#define CAMERA_WIDTH 0.1
#define VIEWING_DISTANCE 0.1
#define SPHERE_RADIUS 0.4

struct OutOfImageException : public std::runtime_error {
	OutOfImageException(int i) : std::runtime_error("Index " + std::to_string(i) + " out of range") {}
};


struct Image;
std::ostream& operator<<(std::ostream& os, const Image &img_obj);

struct Image {
	RGBColour **img;
	int width;
	int height;

	Image(int w, int h) : width(w), height(h) { 
		img = new RGBColour*[height];
		for (int i = 0; i < height; i++) {
			img[i] = new RGBColour[width];
		}
	}

	~Image() {
		for (int i = 0; i < height; i++) {
			delete[] img[i];	
		}
		delete[] img;
	}

	RGBColour *operator[](int i) {
		if (i > height) throw OutOfImageException(i);
		return img[i];
	}

	RGBColour *operator[](int i) const {
		if (i > height) throw OutOfImageException(i);
		return img[i];
	}

	void writeToFile(std::string fname) {
		std::ofstream daFile(fname, std::ios::out | std::ios::binary);
		daFile << *this;
	}
};

std::ostream& operator<<(std::ostream& os, const Image &img_obj) {
	os << "P6" << std::endl << img_obj.width << " " << img_obj.height << std::endl << "255" << std::endl;
	for (int i = 0; i < img_obj.height; i++) {
		for (int j = 0; j < img_obj.width; j++) {
			os.write(&(img_obj[i][j].colour[0]), 3);
		}
	}	
	return os;
}


void trace_dem_rays()
{
	World world(
			Viewport(IMG_SIZE, CAMERA_WIDTH, VIEWING_DISTANCE), // camera/viewport setup
			RGBVec(),					    // background colour (init black)
			Light(vec3(-2.0,2.0,1.0), RGBVec(0.5,0.5,0.5))
	);
	
	Image img(world.viewport.pixelsWide(), world.viewport.pixelsTall());
	
	vec3 sphere_centre(0.0,0.0,20.0);
	Sphere s(sphere_centre, SPHERE_RADIUS, RGBVec(1.0,0.0,0.0)); 
	world.addObject(s);

	for (int i = 0; i < IMG_SIZE; i++) {
		for (int j = 0; j < IMG_SIZE; j++) {
			img[i][j] = world.traceRayAt(i,j);
		}
	}

	img.writeToFile("render.pmm");
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
