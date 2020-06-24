#ifndef LAMBERT_MATERIAL_H
#define LAMBERT_MATERIAL_H

#include "Material.h"

class LambertMaterial : public Material 
{
public:
	void ComputeReflectance(Color& col, const glm::vec3& in, const glm::vec3& out, const Intersection& hit) 
	{
		Color diffuse;
		float reflectance = glm::dot(glm::normalize(in), glm::normalize(out));

		diffuse.Scale(diffuse_color, reflectance);
		col.Multiply(diffuse);
	}

private:
	Color diffuse_color;
};

#endif // LAMBERT_MATERIAL_H