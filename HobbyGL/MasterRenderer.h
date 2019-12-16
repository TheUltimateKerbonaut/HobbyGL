#pragma once

#include "World.h"
#include "Config.h"

#include "SpriteRenderer.h"

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

