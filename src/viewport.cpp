#include "viewport.hpp"
#include <random>
#include "debug.h"

// this constructor is a shorthand to create a square viewport
Viewport::Viewport(int sq_pixels, double sq_across, double viewing_distance) : 
	n_x(sq_pixels), n_y(sq_pixels), d(viewing_distance)
{
	l = -0.5 * sq_across;
	r =  0.5 * sq_across;
	t =  0.5 * sq_across;
	b = -0.5 * sq_across;
	uSpread = (r-l)/static_cast<double>(n_x);
	vSpread = (t-b)/static_cast<double>(n_y);
}

Viewport::Viewport(int pixWidth, int pixHeight, double camera_across, double viewing_distance) :
	n_x(pixWidth), n_y(pixHeight), d(viewing_distance)
{
	double camera_height = camera_across * static_cast<double>(pixHeight)/static_cast<double>(pixWidth);
	l = -0.5 * camera_across;
	r = 0.5 * camera_across;
	t = 0.5 * camera_height;
	b = -0.5 * camera_height;
	uSpread = (r-l)/static_cast<double>(n_x);
	vSpread = (t-b)/static_cast<double>(n_y);
}

double Viewport::uAmount(int i, int ss_level, int ss_iter, bool introduceJitter) {
	
	double across_pixel = (double)i + ((double)ss_iter + 0.5)/(double)ss_level;	
	if (introduceJitter) {
		double jitter = ((((double)rand() / (double)RAND_MAX) - 0.5)) / (2*(double)ss_level);
		across_pixel += jitter;
	}
	return l + uSpread * across_pixel; 
}

double Viewport::vAmount(int j, int ss_level, int ss_iter, bool introduceJitter) {
	double up_pixel = (double)j + ((double)ss_iter + 0.5)/(double)ss_level;	
	if (introduceJitter) {
		double jitter = ((((double)rand() / (double)RAND_MAX) - 0.5)) / (2*(double)ss_level);
		up_pixel += jitter;
	}
	return b + vSpread * up_pixel; 
}

double Viewport::getViewingDistance() { return d; }
double Viewport::getViewingDistance() const { return d; }

int Viewport::pixelsWide() { return n_x; }
int Viewport::pixelsTall() { return n_y; }
