#include <iostream>
#include <fstream>
#include <sstream>

#include "MeshObject.h"
#include "LambertMaterial.h"
#include "glm/glm.hpp"

MeshObject::~MeshObject()
{
	delete[] triangles;
	delete[] vertexes;
}

bool MeshObject::Intersect(const Ray& ray, Intersection& hit) {

	bool success=false;

	for (int i = 0; i < num_tris; ++i)
		if (triangles[i].Intersect(ray, hit)) success = true;

	return success;
}

void MeshObject::MakeBox(float x, float y, float z, Material* mat) {

	vertexes = new Vertex[24];
	num_verts = 24;

	triangles = new Triangle[12];
	num_tris = 12;

	if(mat == nullptr) mat = new LambertMaterial;

	x *= 0.5f;
	y *= 0.5f;
	z *= 0.5f;

	// Corners
	glm::vec3 p000(-x,-y,-z);
	glm::vec3 p001(-x,-y,z);
	glm::vec3 p010(-x,y,-z);
	glm::vec3 p011(-x,y,z);
	glm::vec3 p100(x,-y,-z);
	glm::vec3 p101(x,-y,z);
	glm::vec3 p110(x,y,-z);
	glm::vec3 p111(x,y,z);

	// Tex coords
	glm::vec3 t00(0.0f,0.0f,0.0f);
	glm::vec3 t01(0.0f,1.0f,0.0f);
	glm::vec3 t10(1.0f,0.0f,0.0f);
	glm::vec3 t11(1.0f,1.0f,0.0f);

	// Axis vectors
	glm::vec3 xAxis(1,0,0);
	glm::vec3 yAxis(0,1,0);
	glm::vec3 zAxis(0,0,1);

	// Right
	vertexes[0].Set(p101,xAxis,t00);
	vertexes[1].Set(p100,xAxis,t10);
	vertexes[2].Set(p110,xAxis,t11);
	vertexes[3].Set(p111,xAxis,t01);
	triangles[0].Init(&vertexes[0],&vertexes[1],&vertexes[2],mat);
	triangles[1].Init(&vertexes[0],&vertexes[2],&vertexes[3],mat);

	// Left
	vertexes[4].Set(p000,-xAxis,t00);
	vertexes[5].Set(p001,-xAxis,t10);
	vertexes[6].Set(p011,-xAxis,t11);
	vertexes[7].Set(p010,-xAxis,t01);
	triangles[2].Init(&vertexes[4],&vertexes[5],&vertexes[6],mat);
	triangles[3].Init(&vertexes[4],&vertexes[6],&vertexes[7],mat);

	// Top
	vertexes[8].Set(p011,yAxis,t00);
	vertexes[9].Set(p111,yAxis,t10);
	vertexes[10].Set(p110,yAxis,t11);
	vertexes[11].Set(p010,yAxis,t01);
	triangles[4].Init(&vertexes[8],&vertexes[9],&vertexes[10],mat);
	triangles[5].Init(&vertexes[8],&vertexes[10],&vertexes[11],mat);

	// Bottom
	vertexes[12].Set(p000,-yAxis,t00);
	vertexes[13].Set(p100,-yAxis,t10);
	vertexes[14].Set(p101,-yAxis,t11);
	vertexes[15].Set(p001,-yAxis,t01);
	triangles[6].Init(&vertexes[12],&vertexes[13],&vertexes[14],mat);
	triangles[7].Init(&vertexes[12],&vertexes[14],&vertexes[15],mat);

	// Front
	vertexes[16].Set(p001,zAxis,t00);
	vertexes[17].Set(p101,zAxis,t10);
	vertexes[18].Set(p111,zAxis,t11);
	vertexes[19].Set(p011,zAxis,t01);
	triangles[8].Init(&vertexes[16],&vertexes[17],&vertexes[18],mat);
	triangles[9].Init(&vertexes[16],&vertexes[18],&vertexes[19],mat);

	// Back
	vertexes[20].Set(p100,-zAxis,t00);
	vertexes[21].Set(p000,-zAxis,t10);
	vertexes[22].Set(p010,-zAxis,t11);
	vertexes[23].Set(p110,-zAxis,t01);
	triangles[10].Init(&vertexes[20],&vertexes[21],&vertexes[22],mat);
	triangles[11].Init(&vertexes[20],&vertexes[22],&vertexes[23],mat);
}

std::vector<std::string> split(const std::string& s, char delimiter)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter))
	{
		tokens.push_back(token);
	}
	return tokens;
}

bool MeshObject::LoadPLY(std::string file_path, Material* mat) 
{
	std::ifstream model;
	model.open(file_path);

	if (!model.is_open()) return false;

	int num_v = 0;
	int num_t = 0;
	int line_len = 0;
	bool vert_pos = false;
	bool vert_norm = false;

	std::string line = "";
	while (line.find("end_header") == std::string::npos)
	{
		// get next line.
		std::getline(model, line);

		if (line.find("element vertex") != std::string::npos)
			num_v = std::stoi(line.substr(15, line.size() - 1));

		else if (line.find("element face") != std::string::npos)
			num_t = std::stoi(line.substr(13, line.size() - 1));

		else if (line.find("property") != std::string::npos)
		{
			if (line.find("z") != std::string::npos) vert_pos = true;
			if (line.find("nz") != std::string::npos) vert_norm = true;
		}
	}

	if (num_v <= 0)
	{
		model.close(); 
		return false;
	}

	vertexes = new Vertex[num_v];
	num_verts = num_v;

	for (int i = 0; i < num_v; ++i) {
		std::getline(model, line);
		std::vector<std::string> properties = split(line, ' ');

		size_t size = properties.size();
		if (size >= 3 && vert_pos)
		{
			vertexes[i].position.x = float(std::stof(properties[0]));
			vertexes[i].position.y = float(std::stof(properties[1]));
			vertexes[i].position.z = float(std::stof(properties[2]));
		}
		
		if (size >= 6 && vert_norm)
		{
			vertexes[i].normal.x = float(std::stof(properties[3]));
			vertexes[i].normal.y = float(std::stof(properties[4]));
			vertexes[i].normal.z = float(std::stof(properties[5]));
		}
	}

	if (num_t <= 0)
	{
		model.close();
		return false;
	}

	if (mat == nullptr) mat = new LambertMaterial;

	triangles = new Triangle[num_t];
	num_tris = num_t;
	for (int i = 0; i < num_t; ++i) {

		std::getline(model, line);
		std::stringstream ss(line);

		int count = 0, vert_0 = 0, vert_1 = 0, vert_2 = 0;
		ss >> count >> vert_0 >> vert_1 >> vert_2;

		if (count != 3)
		{
			model.close(); 
			return false;
		}

		triangles[i].Init(&vertexes[vert_0], &vertexes[vert_1], &vertexes[vert_2], mat);
	}

	if (!vert_norm) Smooth();
	model.close();
	return true;
}

void MeshObject::Smooth() 
{
	for (int i = 0; i < num_verts; ++i) vertexes[i].normal = glm::vec3(0);
	for (int i = 0; i < num_tris; ++i)
	{
		glm::vec3 e1 = triangles[i].GetVertex(1).position - triangles[i].GetVertex(0).position;
		glm::vec3 e2 = triangles[i].GetVertex(2).position - triangles[i].GetVertex(0).position;
		glm::vec3 cross = glm::cross(e1, e2);

		for (int j = 0; j < 3; ++j) triangles[i].GetVertex(j).normal += cross;
	}
	for (int i = 0; i < num_verts; ++i) vertexes[i].normal = glm::normalize(vertexes[i].normal);
}

std::pair<size_t, Triangle**> MeshObject::GetTriangles()
{
	return {num_tris, &triangles};
}
