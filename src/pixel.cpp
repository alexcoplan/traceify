#include "pixel.hpp"

Pixel::Pixel(int red, int green, int blue) {
	r = red;
	g = green;
	b = blue;
}

std::string Pixel::getColour() {
	return "(" + std::to_string(r) + "," + std::to_string(g) + "," + std::to_string(b) + ")";
}


