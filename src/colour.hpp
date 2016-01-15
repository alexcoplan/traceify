// colour.hpp
//
// contains two colour representations:
//
// RGBVec is the core representation for internal processing and
// shading calculations
//
// RGBColour is just a lightweight 24-bit colour struct 
// to store the final image in

#ifndef COLOUR_INCLUDE_WARRIOR
#define COLOUR_INCLUDE_WARRIOR

#include <string>
#include <cmath>
#include "vec3.hpp"

// Note: RGBColour is used to store colours for pixels
// RGBVec is used in the internals of the raytracer
// as it offers greater precision

class RGBVec {
private:
	vec3 cvec;
	void normalise();

public:
	RGBVec();
	RGBVec(const RGBVec &rgbv);
	RGBVec(double red, double green, double blue);

	// accessors
	double r();
	double g();
	double b();

	double r() const;
	double g() const;
	double b() const;

	// setters
	void setR(double);
	void setG(double);
	void setB(double);

	// general methods
	void scale(double factor) {
		cvec.scale(factor);
		normalise();
	}
};	

struct RGBColour {
	char colour[3];

public:	
	RGBColour();
	RGBColour(char, char, char);
	std::string getColour();
	RGBColour(const RGBVec &v); 
};


#endif
