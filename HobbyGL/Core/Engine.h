#pragma once

#include "World.h"
#include "Config.h"
#include "Display.h"
#include "../Rendering/MasterRenderer.h"

class Engine
{
public:
	Engine();
	~Engine();

	bool shouldRun();

	void prepare();
	void update(World& world);

	static Config config;
	Display display;
	MasterRenderer masterRenderer;

};

