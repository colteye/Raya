#ifndef BOX_TREE_OBJECT_H
#define BOX_TREE_OBJECT_H

#include "Object.h"
#include "Triangle.h"
#include <string>
#include "MeshObject.h"
#include <vector>

class BoxTreeNode { 
public:
	BoxTreeNode(); 
	~BoxTreeNode(); 

	bool Intersect(const Ray &ray, Intersection &hit);
	bool Intersect_Children(const Ray &ray, Intersection &hit);
	//void Construct_Children(std::vector<Triangle*>& tris, size_t max_depth);

	void Construct(std::vector<Triangle*> tris);

	BoxTreeNode* left;
	BoxTreeNode* right;
	size_t depth = 0;
	
private:

	glm::vec3 box_min, box_max;

	const size_t max_tris = 10;
	std::vector<Triangle*> mesh_tris;

	int split_axis;
	float split_size;
};

class BoxTreeObject : public Object 
{
public:
	BoxTreeObject();
	~BoxTreeObject();
	void Construct(MeshObject& obj);
	bool Intersect(const Ray& ray, Intersection& hit);

private:
	BoxTreeNode* RootNode;
	size_t tree_depth = 7;
};

#endif // BOX_TREE_OBJECT_H
