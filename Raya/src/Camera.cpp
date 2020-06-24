#include "Camera.h"
#include "Ray.h"
#include "Material.h"
#include <thread>

Camera::Camera(int x, int y) : bmp(x, y)
{
	SetResolution(x, y);
}

void Camera::SetFOV(float f)
{
	vertical_fov = glm::radians(f);
	horizontal_fov = 2 * atan(aspect_ratio * tan(vertical_fov / 2));
}

void Camera::SetResolution(int x, int y)
{
	x_resolution = x;
	y_resolution = y;
	aspect_ratio = (float(x_resolution) / float(y_resolution));
	bmp = Bitmap(x_resolution, y_resolution);
}

void Camera::LookAt(glm::vec3 pos, glm::vec3 target, glm::vec3 up)
{
	glm::vec3 d = pos;
	glm::vec3 c = glm::normalize(d - target);
	glm::vec3 a = glm::normalize(glm::cross(up,c));
	glm::vec3 b = glm::cross(c, a);

	world_matrix = glm::mat4x4(a.x, a.y, a.z, 0, b.x, b.y, b.z, 0, c.x, c.y, c.z, 0, d.x, d.y, d.z, 1);
}

void Camera::RenderBatch(int x_start, int x_len, int y_start, int y_len, Scene& s)
{
	for (int y = 0; y < y_len; y++)
	{
		for (int x = 0; x < x_len; x++)
		{
			RenderPixel(x_start + x, y_start + y, s);
		}
	}
}

void Camera::Render(Scene & s)
{ 
	size_t thread_count = 10;//std::thread::hardware_concurrency() / 2;
	size_t x_len = x_resolution / thread_count;

	std::vector<std::thread> threads;

	size_t x_start = 0;
	for (int i = 0; i < thread_count; ++i)
	{
		std::thread t(&Camera::RenderBatch, this, x_start, x_len, 0, y_resolution, std::ref(s));
		threads.push_back(std::move(t));
		x_start += x_len;
	}

	for (std::thread& t : threads)
	{
		t.join();
	}
}



void Camera::RenderPixel(int x, int y, Scene& s)
{
	float fx = (float(x) + 0.5f) / float(x_resolution) - 0.5f;
	float fy = (float(y) + 0.5f) / float(y_resolution) - 0.5f;

	glm::vec3 a = glm::vec3(world_matrix[0]);// NOTE: glmstores matrices as column-major
	glm::vec3 b = glm::vec3(world_matrix[1]);
	glm::vec3 c = glm::vec3(world_matrix[2]);
	glm::vec3 d = glm::vec3(world_matrix[3]);

	float scaleX = 2*tan(horizontal_fov /2);
	float scaleY = 2*tan(vertical_fov /2);

	// create ray.
	Ray ray;
	ray.origin=d;
	ray.direction = glm::normalize(fx*scaleX*a+ fy*scaleY*b - c);

	// run raytrace.
	RayTrace rt(s);
	Intersection hit;
	rt.TraceRay(ray, hit);
	bmp.SetPixel(x, y, hit.shaded_color.ToInt());
}

void Camera::SaveBitmap(std::string file_path)
{
	bmp.SaveBitmap(file_path);
}
