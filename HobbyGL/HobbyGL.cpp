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

static void quitWhenEscape(GLFWwindow* window, int key, int scancode, int action, int mods);

int main()
{
	Engine engine = Engine();
	engine.display.subscribeToInput(quitWhenEscape);

	FirstPersonCamera camera = FirstPersonCamera(engine.display);
	World world = World(camera);

	Light sun = Light(Light::directional, true);
	sun.position = glm::vec3(10, 10, 10);
	sun.colour = glm::vec3(0.6, 0.6, 0.6);
	world.lights.push_back(sun);

	Light sun2 = Light(Light::directional, true);
	sun2.position = glm::vec3(3, 4, -10);
	sun2.colour = glm::vec3(0.8, 0.8, 0.8);
	world.lights.push_back(sun2);

	Sprite sprite = Sprite("pappa.png", 0.5f);
	world.sprites.push_back(sprite);

	GameObject floor = GameObject("plane", "marble.png");
	floor.transform.position.y = -1;
	floor.transform.scale = 10;
	world.gameObjects.push_back(floor);

	GameObject monkey = GameObject("monkey", "white.png");
	world.gameObjects.push_back(monkey);
	monkey.specularFactor = 1;

	GameObject cube = GameObject("cube", "white.png");
	cube.transform.position.x = 3;
	world.gameObjects.push_back(cube);

	while (engine.shouldRun())
	{
		engine.prepare();

		camera.update();

		monkey.transform.rotation.y += 0.1f;

		engine.update(world);
	}

	return 0;
}


static void quitWhenEscape(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
		Engine::config.bloom = !Engine::config.bloom;

	if (key == GLFW_KEY_X && action == GLFW_PRESS)
		Engine::config.dithering = !Engine::config.dithering;

	glfwSetWindowTitle(window, (std::string("HobbyGL - Bloom: ") + std::string((Engine::config.bloom) ? "True" : "False")).c_str());
}