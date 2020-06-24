#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Vertex.h"
#include "Material.h"
#include "Ray.h"

class Triangle 
{
public:
	void Init(Vertex* v0, Vertex* v1, Vertex* v2, Material* m) 
	{
		vertexes[0] = v0; 
		vertexes[1] = v1;
		vertexes[2] = v2;
		mat = m;
	}

	bool Intersect(const Ray& ray, Intersection& hit) const
	{
		glm::vec3 p = ray.origin;
		glm::vec3 neg_d = -ray.direction;
		glm::vec3 a = vertexes[0]->position;
		glm::vec3 b = vertexes[1]->position;
		glm::vec3 c = vertexes[2]->position;

		float det_M = glm::determinant(glm::mat3x3(neg_d, b - a, c - a));
		if (det_M == 0) return false;

		float t = glm::dot((p - a), glm::cross(b - a, c - a)) / det_M;
		float alpha = glm::dot(neg_d, glm::cross(p - a, c - a)) / det_M;
		float beta = glm::dot(neg_d, glm::cross(b - a, p - a)) / det_M;

		if (!(alpha > 0 && beta > 0 && alpha + beta < 1 && t > 0)) return false;

		glm::vec3 pos = a + alpha * (b - a) + beta * (c - a);
		float dist = glm::distance(ray.origin, pos);

		if (dist >= hit.hit_distance) return false;
			
		hit.position = pos;
		hit.hit_distance = dist;

		glm::vec3 a_n = vertexes[0]->normal;
		glm::vec3 b_n = vertexes[1]->normal;
		glm::vec3 c_n = vertexes[2]->normal;

		glm::vec3 norm = glm::normalize(a_n + alpha * (b_n - a_n) + beta * (c_n - a_n));
		if (glm::dot(norm, ray.direction) > 0) norm = -norm;

		hit.normal = norm;
		hit.mat = mat;

		return true;
	}

	Vertex& GetVertex(int i)
	{
		return *vertexes[i];
	}

	glm::vec3 CalculateCenter()
	{
		return (vertexes[0]->position + vertexes[1]->position + vertexes[2]->position) / 3.0f;
	}

private:
	Vertex* vertexes[3];
	Material* mat;
};

#endif // TRIANGLE_H
