#ifndef LIGHT_H
#define LIGHT_H

#include "Color.h"
#include "glm/glm.hpp"

class Light 
{
public:
	Light()									
	{
		intensity = 1.0f; 
		base_color = Color::WHITE;
	}

	void SetBaseColor(const Color& col)		
	{
		base_color = col;
	}

	void SetIntensity(float i)				
	{
		intensity = i;
	}

	virtual float Illuminate(const glm::vec3& pos, Color& col, glm::vec3& to_light, glm::vec3& light_pos) = 0;

protected:
	float intensity;
	Color base_color; // Actual color is Intensity*BaseColor
};

#endif // LIGHT_H