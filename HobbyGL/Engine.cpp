#include "Engine.h"

Engine::Engine() : display(config)
{
	config = Config();
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

Engine::~Engine()
{

}
