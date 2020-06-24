#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "glm/glm.hpp"

#include "Color.h"
class Material;

class Intersection 
{
public:
	Intersection()			
	{
		hit_distance=1e10; 
		mat=nullptr;
	}

	// Ray intersection data
	float hit_distance;
	glm::vec3 position;
	glm::vec3 normal;
	Material* mat;

	// Shaded color
	Color shaded_color;
};

#endif // INTERSECTION_H
