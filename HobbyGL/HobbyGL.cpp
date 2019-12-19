// HobbyGL.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <glad/glad.h>
#include <GLFW\glfw3.h>

#include <iostream>

#include "Core/Config.h"
#include "Core/Display.h"
#include "Core/World.h"
#include "Core/Engine.h"
#include "Core/Camera.h"
#include "Core/Sprite.h"
#include "Core/Texture.h"
#include "Core/Transform.h"

#include "Rendering/Loader.h"
#include "Rendering/MasterRenderer.h"

#include "Utils/FirstPersonCamera.h"

static void quitWhenEscape(GLFWwindow* window);

int main()
{
	Engine engine = Engine();
	engine.display.subscribeToInput(quitWhenEscape);

	FirstPersonCamera camera = FirstPersonCamera(engine.display);
	World world = World(camera);

	Sprite image = Sprite("crateNormal.png");
	world.sprites.push_back(image);

	GameObject barrel = GameObject("barrel", "barrel.png");
	barrel.transform.scale = 0.2;
	world.gameObjects.push_back(barrel);

	while (engine.shouldRun())
	{
		engine.prepare();

		camera.update();

		engine.update(world);
	}

	return 0;
}


static void quitWhenEscape(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}