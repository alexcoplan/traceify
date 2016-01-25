class Viewport {
private:
	int n_x; 		// number of pixels wide
	int n_y; 		// number of pixels tall
	double d;		// viewing (focal) distance
	double l;		// position of left side in u axis
	double r;		// position of right side in u axis
	double b;		// position of bottom side in v axis
	double t;		// position of top side in v axis
	double uSpread; 	// (r-l)/n_x in meters/pixel
	double vSpread;		// (t-b)/n_y in meters/pixel

public:
	// this is a shortcut for initialising a square viewport
	Viewport(int sq_pixels, double sq_across, double viewing_distance);
	Viewport(int pixWidth, int pixHeight, double camera_across, double viewing_distance);

	/* a note about axes:
	 *
	 * u and v are the x and y axes, relative to the camera
	 * w is the z axis
	 *
	 * these functions calculate the amount we should move
	 * in the u and v directions to shoot a ray through a
	 * given pixel */
	double uAmount(int i, int ss_level, int ss_iter);
	double vAmount(int j, int ss_level, int ss_iter);

	double getViewingDistance();
	double getViewingDistance() const; 

	int pixelsWide();
	int pixelsTall();
};
