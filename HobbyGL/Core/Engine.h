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
	void bakeShadows(World& world);
	void bakeReflections(World& world);

	static Config config;
	Display display;
	MasterRenderer masterRenderer;

	// Delta time
	double currentFrameTime;
	double lastFrameTime;
	double timeSinceLastFPSUpdate;
	static float deltaTime;
	static float FPS;

private:

	static void onWindowSizeChange(GLFWwindow* window, int width, int height);

};

