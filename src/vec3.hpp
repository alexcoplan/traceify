// vec3.hpp
//
// core vector class for traceify

#ifndef VEC3_HEADER_WARRIOR
#define VEC3_HEADER_WARRIOR

// include guard ^^

#include <cmath>

class vec3 {
private:
	double vx;
	double vy;
	double vz;

public:
	// constructors
	vec3(double x, double y, double z);
	void operator=(const vec3&);

	void scale(double factor);	// mutates state
	vec3 scaled(double factor);	// preserves state
	vec3 scaled(double factor) const;

	vec3 pointwise(const vec3&);
	vec3 pointwise(const vec3&) const;

	// TODO: add normalised stuff
	vec3 normalised();
	vec3 normalised() const;
	
	double dot(const vec3&);
	double dot(const vec3&) const; 

	double magnitude();
	double magnitude() const;
	
	void debug_print();

	double x();
	double y();
	double z();

	double x() const;
	double y() const;
	double z() const;

	void setX(double);
	void setY(double);
	void setZ(double);
	
	vec3 operator+(const vec3&);
	vec3 operator-(const vec3&);

	vec3 operator+(const vec3&) const;
	vec3 operator-(const vec3&) const;

	vec3 operator+=(const vec3&);
};


// end include guard
#endif

