#ifndef DIRECT_LIGHT_H
#define DIRECT_LIGHT_H

#include "Light.h"

class DirectLight : public Light 
{
public:
	float Illuminate(const glm::vec3 &pos, Color &col, glm::vec3 &toLight, glm::vec3 &ltPos) 
	{
		toLight = -direction; 
		col = base_color; 
		ltPos = pos - (1000000.0f * direction);// Create virtual position
		return intensity;
	}
		
	void SetDirection(glm::vec3 dir)
	{
		direction = glm::normalize(dir);
	}

private:
	glm::vec3 direction;
};

#endif // DIRECT_LIGHT_H