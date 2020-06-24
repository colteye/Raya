#ifndef MESH_OBJECT_H
#define MESH_OBJECT_H

#include <list>
#include <vector>

#include "Object.h"
#include "Triangle.h"
#include <string>

class MeshObject : public Object 
{
public:
	~MeshObject();
	bool Intersect(const Ray& ray, Intersection& hit);
	void MakeBox(float x, float y, float z, Material* mat = nullptr);
	bool LoadPLY(std::string file_path, Material* mat = nullptr);
	void Smooth();

	std::pair<size_t, Triangle**> GetTriangles();

private:
	Vertex* vertexes; 
	size_t num_verts;

	Triangle* triangles; 
	size_t num_tris;
};

#endif // MESH_OBJECT_H