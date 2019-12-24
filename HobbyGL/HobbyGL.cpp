// HobbyGL.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <glad/glad.h>
#include <GLFW\glfw3.h>

#include <iostream>
#include <math.h>

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
	camera.position.y = 3;

	float cameraAngle = 0.0f;
	float distance = 10.0f;
	World world = World(camera);

	Light sun = Light(Light::directional, true);
	sun.position = glm::vec3(10, 10, 10);
	sun.colour = glm::vec3(1.6, 1.6, 1.6);
	world.lights.push_back(sun);

	Sprite sprite = Sprite("pappa.png", 0.5f);
	world.sprites.push_back(sprite);

	GameObject floor = GameObject("plane", "marble.png");
	floor.transform.position.y = -1;
	floor.transform.scale = 10;
	world.gameObjects.push_back(floor);

	GameObject barrel = GameObject("barrel", "barrel.png", "barrelNormal.png");
	barrel.transform.scale = 0.2f;
	world.gameObjects.push_back(barrel);

	while (engine.shouldRun())
	{
		engine.prepare();

		camera.update();

		barrel.transform.rotation.y += 5.0f * Engine::deltaTime;

		/*cameraAngle += 0.5f * Engine::deltaTime;
		camera.position.x = distance * std::sin(cameraAngle) * std::cos(distance);
		camera.position.z = distance * std::cos(cameraAngle) * std::sin(distance);

		camera.yaw = -glm::degrees(cameraAngle) - 180;
		camera.pitch = 20;*/

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
}