#include "colour.hpp"

// RGBVec: constructors
RGBVec::RGBVec() : cvec(0.0,0.0,0.0) {} // default to black
RGBVec::RGBVec(const vec3 &v) : cvec(v) {}
RGBVec::RGBVec(const RGBVec &rgbv) : cvec(rgbv.cvec) {}
RGBVec::RGBVec(double r, double g, double b) : cvec(r,g,b) {}

// RGBVec: accessors
double RGBVec::r() { return cvec.x(); }
double RGBVec::g() { return cvec.y(); }
double RGBVec::b() { return cvec.z(); }

double RGBVec::r() const { return cvec.x(); }
double RGBVec::g() const { return cvec.y(); }
double RGBVec::b() const { return cvec.z(); }

void RGBVec::setR(double r) { cvec.setX(r); }
void RGBVec::setG(double g) { cvec.setY(g); }
void RGBVec::setB(double b) { cvec.setZ(b); }

// RGBVec: private methods
void RGBVec::normalise() {
	// simply clip the colour
	if (r() > 1.0) setR(1.0);
	if (g() > 1.0) setG(1.0);
	if (b() > 1.0) setB(1.0);
}

void RGBVec::scale(double factor) {
	cvec.scale(factor);
	normalise();
}

RGBVec RGBVec::scaled(double factor) {
	return RGBVec(cvec.scaled(factor));
}

RGBVec RGBVec::scaled(double factor) const {
	return RGBVec(cvec.scaled(factor));
}

RGBVec RGBVec::multiplyColour(const RGBVec &v) {
	return RGBVec(r() * v.r(), g() * v.g(), b() * v.b());
}

RGBVec RGBVec::multiplyColour(const RGBVec &v) const {
	return RGBVec(r() * v.r(), g() * v.g(), b() * v.b());
}

D(
void RGBVec::debug_print() {
	std::cout << "(" << r() << "," << g() << "," << b() << ")" << std::endl;
}
)

// RGBColour
RGBColour::RGBColour() {
	colour[0] = 0;
	colour[1] = 0;
 	colour[2] = 0;
};


RGBColour::RGBColour(char red, char green, char blue) {
	colour[0] = red;
	colour[1] = green;
	colour[2] = blue;
}	

RGBColour::RGBColour(const RGBVec &v) { 
	colour[0] = (char)(round(v.r() * 255.0));
	colour[1] = (char)(round(v.g() * 255.0));
	colour[2] = (char)(round(v.b() * 255.0));
}

std::string RGBColour::getColour() {
	return "(" + std::to_string(colour[0]) + "," + std::to_string(colour[1]) + "," + std::to_string(colour[2]) + ")";
}
