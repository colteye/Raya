#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include "glm/glm.hpp"

#include "Intersection.h"
#include "Color.h"

class Material
{
public:
	virtual void ComputeReflectance(Color& col, const glm::vec3& in, const glm::vec3& out, const Intersection& hit) = 0;
};

#endif // MATERIAL_H