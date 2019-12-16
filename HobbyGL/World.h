#pragma once

#include <vector>

#include "GameObject.h"
#include "Sprite.h"

class World
{
public:

	std::vector<std::reference_wrapper<GameObject>> gameObjects;
	std::vector<std::reference_wrapper<Sprite>> sprites;

};