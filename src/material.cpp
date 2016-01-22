#include <cmath>
#include "material.hpp"

Material::Material(const Material& m) {
	diffuse = m.diffuse;
	specularity = m.specularity;
	ambient = m.ambient;
	material_colour = m.material_colour;
	specular_colour = m.specular_colour;
}

// No Shader (constant colour => Full Ambient Only)
Material::Material(const RGBVec &colour) : 
	diffuse(false), specularity(0.0), ambient(1.0), material_colour(colour) {}

// No Specular (Diffuse + [Ambient])
Material::Material(const RGBVec &colour, double ambAmount) :
	diffuse(true), specularity(0.0), ambient(ambAmount), material_colour(colour) {}

// Full Shader with implicit white specular colour (Diffuse + Shader + [Ambient])
Material::Material(const RGBVec &matcolour, double specAmount, double ambAmount) :
	diffuse(true), specularity(specAmount), ambient(ambAmount),
	material_colour(matcolour), specular_colour(1.0,1.0,1.0) {}

// Full Shader (Diffuse + Specular + [Ambient])
Material::Material(const RGBVec &matcolour, const RGBVec &speccolour, double specAmount, double ambAmount) :
	diffuse(true), 
	specularity(specAmount), 
	ambient(ambAmount), 
	material_colour(matcolour), 
	specular_colour(speccolour) {}

RGBVec Material::shade(const Light &light, const vec3 &n, const vec3 &v, const vec3 &l) const {
	RGBVec result = material_colour.scaled(ambient);
	if (diffuse) 
		result += material_colour.multiplyColour(light.colour).scaled(n.dot(l));
	if (specularity > 0.0) {
		vec3 h = (v + l).normalised();
		double coefficient = pow( n.dot(h), specularity );
		result += specular_colour.multiplyColour(light.colour).scaled(coefficient);
	}
	return result;
}

// since we don't want to duplicate the above code, we use a const_cast
// to do the const overload
RGBVec Material::shade(const Light &light, const vec3 &n, const vec3 &v, const vec3 &l) {
	return const_cast<const Material*>(this)->shade(light,n,v,l);
}

void Material::setMaterialColour(const RGBVec &c) {
	material_colour = c;
}

void Material::operator=(const Material &m) {
	diffuse = m.diffuse;
	specularity = m.specularity;
	ambient = m.ambient;
	material_colour = m.material_colour;
	specular_colour = m.specular_colour;
}

