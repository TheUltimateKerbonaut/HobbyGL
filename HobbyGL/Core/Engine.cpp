#include "Engine.h"

#include "../Rendering/Loader.h"

Config Engine::config;

Engine::Engine() : display(config)
{
	config = Config();
	display.subscribeToWindowChange(onWindowSizeChange);
}

bool Engine::shouldRun()
{
	return !display.windowShouldClose();
}

void Engine::prepare()
{
	display.beginFrame();
	masterRenderer.prepareFrame(config);
}

void Engine::update(World& world)
{
	masterRenderer.renderFrame(world, config);
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
