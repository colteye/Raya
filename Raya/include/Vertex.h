#ifndef VERTEX_H
#define VERTEX_H

#include "glm/glm.hpp"

class Vertex 
{
public:
	void Set(const glm::vec3& p, const glm::vec3& n, const glm::vec3& t)
	{
		position = p; 
		normal = n; 
		tex_coord = t;
	}

	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 tex_coord;
};

#endif // VERTEX_H