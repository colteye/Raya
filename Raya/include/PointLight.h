#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "Light.h"

#pragma once

class PointLight : public Light 
{
public:

	float Illuminate(const glm::vec3 &pos, Color &col, glm::vec3 &toLight, glm::vec3 &ltPos) 
	{
		toLight = position - pos; 
		float bright = intensity / powf(glm::length(toLight), 2.0f); // Inverse square falloff
		toLight=glm::normalize(toLight);
		col = base_color;
		ltPos= position;
		return bright;
	}

	void SetPosition(glm::vec3 pos)
	{
		position = pos;
	}

private:
	glm::vec3 position;
};

#endif // POINT_LIGHT_H