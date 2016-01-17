#include <fstream>
#include <string>
#include "colour.hpp"

struct Image {
	RGBColour **img;
	int width;
	int height;

	Image(int w, int h);
	~Image();
	RGBColour *operator[](int i);
	RGBColour *operator[](int i) const;
	void writeToFile(std::string fname);
};

std::ostream& operator<<(std::ostream& os, const Image &img);

struct OutOfImageException : public std::runtime_error {
	OutOfImageException(int i);
};
