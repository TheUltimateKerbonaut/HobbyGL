#pragma once

#include "..//Core/World.h"
#include "../Core/Config.h"

#include "RenderShaderPrograms/SpriteRenderer.h"

#include <map>
#include <vector>

class MasterRenderer
{
public:
	MasterRenderer();
	~MasterRenderer();

	void prepareFrame(Config& config);
	void renderFrame(World& world, Config& config);

private:
	SpriteRenderer spriteRenderer;

};

