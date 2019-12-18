#pragma once

#include <GLFW/glfw3.h>

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

private:

	static void onWindowSizeChange(GLFWwindow* window, int width, int height);

};

