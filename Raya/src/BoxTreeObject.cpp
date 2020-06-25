#define _CRT_SECURE_NO_WARNINGS

#include "LambertMaterial.h"
#include "glm/glm.hpp"
#include "BoxTreeObject.h"
#include <algorithm>
#include <queue>

#include <chrono> 
////////////////////////////////////////////////////////////
// BOX TREE NODE STUFF /////////////////////////////////////
////////////////////////////////////////////////////////////

BoxTreeNode::BoxTreeNode() 
{ 
	left = nullptr;
	right = nullptr;
}

BoxTreeNode::~BoxTreeNode()
{
	if (left != nullptr) delete left;
	if (right != nullptr) delete right;
}

bool BoxTreeNode::Intersect(const Ray& ray, Intersection& hit)
{
	glm::vec3 t_1 = (box_min - ray.origin) / ray.direction;
	glm::vec3 t_2 = (box_max - ray.origin) / ray.direction;

	float t_min = std::max({ std::min({t_1.x, t_2.x}), std::min({t_1.y, t_2.y}), std::min({t_1.z, t_2.z}) });
	float t_max = std::min({ std::max({t_1.x, t_2.x}), std::max({t_1.y, t_2.y}), std::max({t_1.z, t_2.z}) });

	if (t_max < 0) return false;
	if (t_min > t_max) return false;

	if (t_min >= 0)
	{
		if (t_min >= hit.hit_distance) return false;
		hit.hit_distance = t_min;
		return true;
	}

	if (t_max >= hit.hit_distance) return false;
	hit.hit_distance = t_max;
	return true;
}

/*bool BoxTreeNode::Intersect_Children(const Ray& ray, Intersection& hit)
{
	if (left == nullptr && right == nullptr)
	{
		bool success = false;
		for (Triangle* t : mesh_tris)
		{
			if (t->Intersect(ray, hit)) success = true;
		}
		return success;
	}

	std::vector<std::pair<float, BoxTreeNode*>> node_hits;
	Intersection inter_l, inter_r;

	if (left != nullptr)
	{
		inter_l.hit_distance = hit.hit_distance;

		if (left->Intersect(ray, inter_l))
			node_hits.push_back({ inter_l.hit_distance, left });
	}

	if (right != nullptr)
	{
		inter_r.hit_distance = hit.hit_distance;

		if (right->Intersect(ray, inter_r))
			node_hits.push_back({ inter_r.hit_distance, right });
	}

	if (node_hits.size() > 1)
	{
		if (inter_r.hit_distance < inter_l.hit_distance)
		{
			node_hits.clear();
			node_hits.push_back({ inter_r.hit_distance, right });
			node_hits.push_back({ inter_l.hit_distance, left });
		}
	}

	bool success = false;
	for (int i = 0; i < node_hits.size(); ++i)
		if (node_hits[i].first < hit.hit_distance)
		{
			if (node_hits[i].second->Intersect_Children(ray, hit)) success = true;
		}
		
	return success;
}*/

std::vector<BoxTreeNode*> BoxTreeNode::Intersect_Children(const Ray& ray, Intersection& hit)
{
	std::vector<BoxTreeNode*> node_hits;
	Intersection inter_l, inter_r;

	inter_l.hit_distance = inter_r.hit_distance = hit.hit_distance;

	if (left->Intersect(ray, inter_l))
		node_hits.push_back(left);

	if (right->Intersect(ray, inter_r))
		node_hits.push_back(right);

	if (node_hits.size() == 2)
	{
		if (inter_r.hit_distance < inter_l.hit_distance)
		{
			node_hits[0] = right;
			node_hits[1] = left;
		}
	}
	return node_hits;
}

void BoxTreeNode::Construct(std::vector<Triangle*> tris)
{
	box_max = glm::vec3{ FLT_MIN, FLT_MIN, FLT_MIN };
	box_min = glm::vec3{ FLT_MAX, FLT_MAX, FLT_MAX };

	for (Triangle* t : tris)
	{
		for (int j = 0; j < 3; ++j)
		{
			glm::vec3 vert_pos = t->GetVertex(j).position;

			if (vert_pos.x < box_min.x) box_min.x = vert_pos.x;
			if (vert_pos.y < box_min.y) box_min.y = vert_pos.y;
			if (vert_pos.z < box_min.z) box_min.z = vert_pos.z;

			if (vert_pos.x > box_max.x) box_max.x = vert_pos.x;
			if (vert_pos.y > box_max.y) box_max.y = vert_pos.y;
			if (vert_pos.z > box_max.z) box_max.z = vert_pos.z;
		}
	}

	float x_size = abs(box_min.x - box_max.x);
	float y_size = abs(box_min.y - box_max.y);
	float z_size = abs(box_min.z - box_max.z);

	split_size = std::max({ x_size , y_size, z_size });

	if (split_size == x_size) split_axis = 0;
	else if (split_size == y_size) split_axis = 1;
	else split_axis = 2;

	mesh_tris = tris;
}

void BoxTreeNode::Construct_Children(size_t max_tris)
{
	std::vector<Triangle*> tris_1, tris_2;
	float split_line = box_min[split_axis] + (split_size / 2);

	for (Triangle* t : mesh_tris)
	{
		glm::vec3 center = t->CalculateCenter();
		if (center[split_axis] > split_line)
		{
			tris_1.push_back(t);
		}
		else tris_2.push_back(t);
	}

	// clear mesh tris from this node.
	std::vector<Triangle*>().swap(mesh_tris);

	// tris 1 corresponds to left
	if (tris_1.empty())
	{
		tris_1 = std::vector<Triangle*>(tris_2.end() - max_tris, tris_2.end());
		tris_2.erase(tris_2.end() - max_tris, tris_2.end());
	}

	// tris 2 corresponds to right
	if (tris_2.empty())
	{
		tris_2 = std::vector<Triangle*>(tris_1.end() - max_tris, tris_1.end());
		tris_1.erase(tris_1.end() - max_tris, tris_1.end());
	}

	left = new BoxTreeNode();
	left->Construct(tris_1);

	right = new BoxTreeNode();
	right->Construct(tris_2);
}

///////////////////////////////////////////////////////
// BOX TREE STUFF /////////////////////////////////////
///////////////////////////////////////////////////////

BoxTreeObject::BoxTreeObject()
{
	RootNode = new BoxTreeNode();
}

BoxTreeObject::~BoxTreeObject()
{
	delete RootNode;
}

void BoxTreeObject::Construct(MeshObject& obj)
{
	std::pair<size_t, Triangle**> tris = obj.GetTriangles();
	std::vector<Triangle*> tri_ptrs;

	// convert to triangle pointer vector for dynamic array.
	for (int i = 0; i < tris.first; ++i)
		tri_ptrs.push_back(&(*tris.second)[i]);

	RootNode->Construct(tri_ptrs);
	std::queue<BoxTreeNode*> q;

	q.push(RootNode);
  
	while (!q.empty())
	{
		BoxTreeNode* node = q.front();

		size_t size = node->GetTriangles().size();
		if (size > max_tris_node)
		{
			node->Construct_Children(max_tris_node);
			q.push(node->left);
			q.push(node->right);
		}
		q.pop();
	}
}

bool BoxTreeObject::Intersect(const Ray& ray, Intersection& hit)
{
	if (!RootNode->Intersect(ray, hit)) return false;
	hit.hit_distance = 1e10f;

	bool success = false;

	std::vector<BoxTreeNode*> node_stack;
	node_stack.push_back(RootNode);
	int current_index = 1;

	while (current_index > 0)
	{
		--current_index;
		BoxTreeNode* node = node_stack[current_index];
		node_stack.pop_back();

		if (node->IsLeaf())
		{
			for (Triangle* t : node->GetTriangles())
				if (t->Intersect(ray, hit)) success = true;
			continue;
		}

		std::vector<BoxTreeNode*> node_hits = node->Intersect_Children(ray, hit);
		for (int i = node_hits.size() - 1; i >= 0; --i)
		{
			node_stack.push_back(node_hits[i]);
			current_index++;
		}
	}
	return success;
}