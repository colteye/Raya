#ifndef RAY_TRACE_H
#define RAY_TRACE_H

#include <list>
#include <vector>

#include "Scene.h"
#include "Triangle.h"
#include <string>

class RayTrace
{
public:
	RayTrace(Scene& s);
	bool TraceRay(const Ray& ray, Intersection& hit, int depth=1);

private:
	Scene* scn;
	size_t max_depth;

	size_t primary_rays = 0;
	size_t secondary_rays = 0;
	size_t shadow_rays = 0;
};

#endif // RAY_TRACE_H