#pragma once

#include <vector>

#include "Camera.h"
#include "GameObject.h"
#include "Sprite.h"
#include "Light.h"

class World
{
public:

	inline World(Camera& _camera) : camera(_camera)
	{
		
	}

	Camera& camera;
	std::vector<std::reference_wrapper<GameObject>> gameObjects;
	std::vector<std::reference_wrapper<Sprite>> sprites;
	std::vector<std::reference_wrapper<Light>> lights;

};