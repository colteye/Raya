#include "RayTrace.h"

RayTrace::RayTrace(Scene & s)
{
	scn = &s;
}

bool RayTrace::TraceRay(const Ray& ray, Intersection& hit, int depth)
{
	if (!scn->Intersect(ray, hit)) 
	{ 
		hit.shaded_color = scn->GetSkyColor(); 
		return false; 
	}

	hit.shaded_color = Color::BLACK;
	for (int i = 0; i < scn->GetNumLights(); ++i)
	{
		// Light calculations.
		Color light_color;
		glm::vec3 to_light, light_pos;
		float illum = scn->GetLight(i).Illuminate(hit.position, light_color, to_light, light_pos);

		if (illum <= 0 || glm::dot(hit.normal, to_light) < 0) continue;

		light_color.Scale(light_color, illum);
		hit.mat->ComputeReflectance(light_color, to_light, hit.normal, hit);

		// Shadow calculations.
		float shadow_bias = 0.002f;

		Intersection shadow_hit;
		shadow_hit.hit_distance = glm::distance(ray.origin, light_pos) - shadow_bias;

		Ray shadow_ray;
		shadow_ray.origin = hit.position + (hit.normal * shadow_bias);
		shadow_ray.direction = to_light;

		if (!scn->Intersect(shadow_ray, shadow_hit)) hit.shaded_color.Add(light_color);
	}

	if (depth == max_depth) return true;
	return true;
}
