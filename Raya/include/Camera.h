#ifndef CAMERA_H
#define CAMERA_H

#include "Bitmap.h"
#include "Scene.h"
#include "glm/glm.hpp"

class Camera
{
public:
	Camera(int x, int y);
	void SetFOV(float f);
	void SetResolution(int x, int y);
	void LookAt(glm::vec3 pos, glm::vec3 target, glm::vec3 up);
	void Render(Scene& s);

	void SaveBitmap(std::string file_path);

private:
	void RenderPixel(int x, int y, Scene& s);

	int x_resolution, y_resolution; 
	glm::mat4x4 world_matrix; 
	float vertical_fov, horizontal_fov;
	float aspect_ratio; 
	Bitmap bmp;
};

#endif // CAMERA_H
