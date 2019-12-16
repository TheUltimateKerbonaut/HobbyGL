#pragma once

#include "World.h"
#include "Config.h"
#include "Display.h"
#include "MasterRenderer.h"

class Engine
{
public:
	Engine();
	~Engine();

	bool shouldRun();

	void prepare();
	void update(World& world);

	Config config;
	Display display;
	MasterRenderer masterRenderer;

};

