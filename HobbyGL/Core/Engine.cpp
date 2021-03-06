#include "Engine.h"

#include "../Rendering/Loader.h"

Config Engine::config;

float Engine::deltaTime;
float Engine::FPS;

Engine::Engine() : display(config), masterRenderer(display)
{
	config = Config();
	display.subscribeToWindowChange(onWindowSizeChange);

	currentFrameTime = glfwGetTime();
	lastFrameTime = currentFrameTime;
	FPS = 0;
}

void Engine::bakeShadows(World& world)
{
	masterRenderer.bakeShadows(world, config);
}

void Engine::bakeReflections(World& world)
{
	masterRenderer.bakeReflections(world, config);
}

bool Engine::shouldRun()
{
	return !display.windowShouldClose();
}

void Engine::prepare()
{
	display.beginFrame();
	masterRenderer.prepareFrame(config);

	currentFrameTime = glfwGetTime();
	deltaTime = (float)(currentFrameTime - lastFrameTime);
	lastFrameTime = currentFrameTime;

	if (timeSinceLastFPSUpdate >= 50.0f)
	{
		FPS = (float)((int)((deltaTime * 1000) * 100)) / 100.0f;
		timeSinceLastFPSUpdate = 0.0f;
	}
	else { timeSinceLastFPSUpdate++; }
}

void Engine::update(World& world)
{
	masterRenderer.renderFrame(world, config);

	masterRenderer.endFrame(config, world);

	display.update();
}

void Engine::onWindowSizeChange(GLFWwindow* window, int width, int height)
{
	config.width = width;
	config.height = height;
}

Engine::~Engine()
{
	Loader::close();
}
