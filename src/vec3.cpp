#include "vec3.hpp"


// DEBUG

#include <string>
#include <iostream>
void vec3::debug_print() {
	std::cout << "(" << vx << "," << vy << "," << vz << ")" << std::endl;
};

// END DEBUG

	
vec3::vec3(double x, double y, double z) : vx(x), vy(y), vz(z) {}

void vec3::operator=(const vec3& v) {
	vx = v.vx;
	vy = v.vy;
	vz = v.vz;
}	
       			      
void vec3::scale(double factor) {
	vx *= factor;
	vy *= factor;
	vz *= factor;
}

vec3 vec3::scaled(double factor) {
	return vec3(vx*factor, vy*factor, vz*factor);
}

vec3 vec3::scaled(double factor) const {
	return vec3(vx*factor, vy*factor, vz*factor);
}

vec3 vec3::normalised() {
	return scaled(1.0/magnitude());
}

vec3 vec3::normalised() const {
	return scaled(1.0/magnitude());
}

// scalar product (dot product)
double vec3::dot(const vec3& v) {
	return vx * v.vx + vy * v.vy + vz * v.vz;
}

double vec3::dot(const vec3& v) const {
	return vx * v.vx + vy * v.vy + vz * v.vz;
}

// magnitude of vector
double vec3::magnitude() {
	return sqrt(vx*vx + vy*vy + vz*vz);
}

double vec3::magnitude() const {
	return sqrt(vx*vx + vy*vy + vz*vz);
}

vec3 vec3::operator+(const vec3& v) {
	return vec3(vx + v.vx, vy + v.vy, vz + v.vz);
}

vec3 vec3::operator+(const vec3& v) const {
	return vec3(vx + v.vx, vy + v.vy, vz + v.vz);
}

vec3 vec3::operator-(const vec3& v) {
	return vec3(vx - v.vx, vy - v.vy, vz - v.vz);
}

vec3 vec3::operator-(const vec3& v) const {
	return vec3(vx - v.vx, vy - v.vy, vz - v.vz);
}

double vec3::x() { return vx; }
double vec3::y() { return vy; }
double vec3::z() { return vz; }

double vec3::x() const { return vx; }
double vec3::y() const { return vy; }
double vec3::z() const { return vz; }

void vec3::setX(double x) { vx = x; }
void vec3::setY(double y) { vy = y; }
void vec3::setZ(double z) { vz = z; }
