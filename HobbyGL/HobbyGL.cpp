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
#include "Core/Light.h"

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

	Light sun = Light(Light::directional);
	sun.position = glm::vec3(1, 1, 1);
	sun.colour = glm::vec3(1, 1, 1);
	world.lights.push_back(sun);

	Sprite sprite = Sprite("pappa.png", 0.5f);
	world.sprites.push_back(sprite);

	GameObject floor = GameObject("plane", "marble.png");
	floor.transform.position.y = -1;
	floor.transform.scale = 10;
	world.gameObjects.push_back(floor);

	GameObject monkey = GameObject("monkey", "white.png");
	world.gameObjects.push_back(monkey);
	monkey.specularFactor = 1;

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