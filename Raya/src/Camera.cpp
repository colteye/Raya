#include "Camera.h"
#include "Ray.h"
#include "Material.h"

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

void Camera::Render(Scene & s)
{ 
	for (int y = 0; y < y_resolution; y++)
	{ 
		for (int x = 0; x < x_resolution; x++)
		{ 
			RenderPixel(x, y, s); 
		} 
	} 
}

void Camera::RenderPixel(int x, int y, Scene & s)
{
	float fx = (float(x) + 0.5f) / float(x_resolution) - 0.5f;
	float fy = (float(y) + 0.5f) / float(y_resolution) - 0.5f;

	glm::vec3 a = glm::vec3(world_matrix[0]);// NOTE: glmstores matrices as column-major
	glm::vec3 b = glm::vec3(world_matrix[1]);
	glm::vec3 c = glm::vec3(world_matrix[2]);
	glm::vec3 d = glm::vec3(world_matrix[3]);

	float scaleX = 2*tan(horizontal_fov /2);
	float scaleY = 2*tan(vertical_fov /2);

	Ray ray;
	ray.origin=d;
	ray.direction = glm::normalize(fx*scaleX*a+ fy*scaleY*b - c);

	Intersection hit;
	if (s.Intersect(ray, hit))
	{
		hit.shaded_color = Color::BLACK;
		for (int i = 0; i < s.GetNumLights(); ++i)
		{
			// Light calculations.
			Color light_color;
			glm::vec3 to_light, light_pos;
			float illum = s.GetLight(i).Illuminate(hit.position, light_color, to_light, light_pos);

			light_color.Scale(light_color, illum);
			hit.mat->ComputeReflectance(light_color, to_light, hit.normal, hit);
			hit.shaded_color.Add(light_color);

			// Shadow calculations.
			/*Ray shadow_ray;
			float shadow_bias = 0.04f;
			shadow_ray.origin = hit.position + (hit.normal * shadow_bias);
			shadow_ray.direction = to_light;

			Color shadow_col;
			shadow_col.Scale(Color::WHITE, 1 - illum);
			if (s.Intersect(shadow_ray, hit)) hit.shaded_color.Multiply(shadow_col);*/

		}
		bmp.SetPixel(x, y, hit.shaded_color.ToInt());
	}
	else
	{
		bmp.SetPixel(x, y, s.GetSkyColor().ToInt());
	}
}

void Camera::SaveBitmap(std::string file_path)
{
	bmp.SaveBitmap(file_path);
}
