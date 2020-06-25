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
	//bool Intersect_Children(const Ray &ray, Intersection &hit);
	std::vector<BoxTreeNode*> Intersect_Children(const Ray& ray, Intersection& hit);

	void Construct(std::vector<Triangle*> tris);
	void Construct_Children(size_t max_tris);


	BoxTreeNode* left;
	BoxTreeNode* right;

	std::vector<Triangle*>& GetTriangles() { return mesh_tris; }
	bool IsLeaf() { return left == nullptr && right == nullptr; }
	
private:

	glm::vec3 box_min, box_max;
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
	const size_t max_tris_node = 40;
};

#endif // BOX_TREE_OBJECT_H