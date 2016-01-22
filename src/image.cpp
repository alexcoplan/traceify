#include "image.hpp"

OutOfImageException::OutOfImageException(int i) :
	std::runtime_error("Index " + std::to_string(i) + " is out of image") {}

Image::Image(int w, int h) : width(w), height(h) {
	img = new RGBColour*[width];
	for (int i = 0; i < width; i++) {
		img[i] = new RGBColour[height];
	}
}

Image::~Image() {
	for (int i = 0; i < height; i++) {
		delete[] img[i];
	}
	delete[] img;
}

RGBColour *Image::operator[](int i) {
	if (i > height) throw OutOfImageException(i);
	return img[i];
}

RGBColour *Image::operator[](int i) const {
	if (i > height) throw OutOfImageException(i);
	return img[i];
}

void Image::writeToFile(std::string fname) {
	std::ofstream daFile(fname, std::ios::out | std::ios::binary);
	daFile << *this;
}

std::ostream& operator<<(std::ostream& os, const Image &img_obj) {
	os << "P6" << std::endl << img_obj.width << " " << img_obj.height << std::endl << "255" << std::endl;
	for (int i = 0; i < img_obj.height; i++) {
		for (int j = 0; j < img_obj.width; j++) {
			os.write(&(img_obj[j][img_obj.height - i - 1].colour[0]), 3);
		}
	}	
	return os;
}

