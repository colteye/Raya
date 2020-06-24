#ifndef LAMBERT_MATERIAL_H
#define LAMBERT_MATERIAL_H

#include "Material.h"

class LambertMaterial : public Material 
{
public:
	void ComputeReflectance(Color& col, const glm::vec3& in, const glm::vec3& out, const Intersection& hit) 
	{
		Color diffuse_tmp;
		float reflectance = glm::dot(glm::normalize(in), glm::normalize(out));

		diffuse_tmp.Scale(diffuse_color, reflectance);
		col.Multiply(diffuse_tmp);
	}

private:
	Color diffuse_color;
};

#endif // LAMBERT_MATERIAL_H