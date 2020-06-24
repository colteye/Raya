
#include "InstanceObject.h"
#include "LambertMaterial.h"
#include "glm/glm.hpp"

#include <iostream>

InstanceObject::InstanceObject(Object & obj)
{
	SetChild(obj);
}

bool InstanceObject::Intersect(const Ray &ray, Intersection &hit)
{
	Ray ray2; 
	ray2.origin = glm::vec3(matrix_inverse * glm::vec4(ray.origin, 1));
	ray2.direction = glm::vec3(matrix_inverse * glm::vec4(ray.direction, 0));

	if (!(child->Intersect(ray2, hit))) return false; 
	
	hit.position = glm::vec3(matrix * glm::vec4(hit.position, 1)); 
	hit.normal = glm::vec3(matrix * glm::vec4(hit.normal, 0)); 
	hit.hit_distance = glm::distance(ray.origin, hit.position); // Correct for any scaling

	return true;
}

void InstanceObject::SetChild(Object & obj)
{
	child = &obj;
}

void InstanceObject::SetMatrix(glm::mat4x4 & mtx)
{
	matrix = mtx;
	matrix_inverse = glm::inverse(matrix);
}
