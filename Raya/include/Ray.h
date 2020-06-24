#ifndef RAY_H
#define RAY_H

#include "glm/glm.hpp"

class Ray 
{
public:
	glm::vec3 origin;
	glm::vec3 direction;
};

#endif // RAY_H