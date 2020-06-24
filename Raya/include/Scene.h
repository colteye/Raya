#ifndef SCENE_H
#define SCENE_H

#include "Object.h"
#include "Light.h"
#include <vector>

class Scene {
public:
	Scene()
	{
		sky_color.Set(0.2f,0.2f,0.5f);
	}

	void AddObject(Object& obj)
	{
		objects.push_back(&obj);
	}

	void AddLight(Light& lgt)
	{
		lights.push_back(&lgt);
	}

	void SetSkyColor(const Color sky)
	{
		sky_color=sky;
	}

	size_t GetNumLights()
	{
		return lights.size();
	}

	Light &GetLight(int i)
	{
		return *lights[i];
	}

	Color GetSkyColor()
	{
		return sky_color;
	}

	bool Intersect(const Ray& ray,Intersection& hit) 
	{
		bool success=false;

		for(int i = 0; i < objects.size(); ++i)
			if(objects[i]->Intersect(ray,hit)) success=true;

		return success;
	}

private:
	std::vector<Object*> objects;
	std::vector<Light*> lights;
	Color sky_color;
};

#endif // SCENE_H