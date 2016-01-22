#include "colour.hpp"
#include "light.hpp"

class Material {
private:
	bool diffuse;

	double specularity;		// by convention, let specularity = 0.0 => no specular shading
	double ambient;			// this convention is not necessary here (although it holds)

	RGBVec material_colour;	// diffuse (and ambient colour)
	RGBVec specular_colour;

public:
	Material(const Material&);

	// No Shader (constant colour => Full Ambient Only)
	Material(const RGBVec &base_colour);
	
	// No Specular (Diffuse + [Ambient])
	Material(const RGBVec &base_colour, double ambient);

	// Full Shader with implicit white specularity
	Material(const RGBVec &colour, double spec, double amb);	

	// Full Shader (Diffuse + Specular + [Ambient])
	Material(const RGBVec &matcol, const RGBVec &speccol, double spec, double amb);
	
	RGBVec shade(const Light &light, const vec3 &n, const vec3 &v, const vec3 &l);
	RGBVec shade(const Light &light, const vec3 &n, const vec3 &v, const vec3 &l) const;

	void setMaterialColour(const RGBVec &c); 

	void operator=(const Material&);
};
