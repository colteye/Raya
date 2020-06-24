#ifndef INSTANCE_OBJECT_H
#define INSTANCE_OBJECT_H

#include "Object.h"
#include "Triangle.h"

class InstanceObject : public Object
{ 
public:
	InstanceObject(Object& obj);
	bool Intersect(const Ray& ray, Intersection& hit); 
	void SetChild(Object& obj); 
	void SetMatrix(glm::mat4x4& mtx); 
private:
	glm::mat4x4 matrix; 
	glm::mat4x4 matrix_inverse; // Pre-computed inverse of Matrix
	Object* child;
};

#endif // INSTANCE_OBJECT_H
