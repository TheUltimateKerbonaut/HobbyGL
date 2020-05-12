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
	bool releaseControl = false;
	World world = World(camera);


	float brightness = 3.0f;

	Light sun = Light(Light::point, true);
	sun.position = glm::vec3(5, 1, 5);
	sun.colour = glm::vec3(0.6f * brightness, 0.6f * brightness, 0.35f * brightness);
	world.lights.push_back(sun);

	Light sun2 = Light(Light::point, true);
	sun2.position = glm::vec3(-5, 1, -5);
	sun2.colour = glm::vec3(0.3 * brightness, 0.8 * brightness, 0.8 * brightness);

	world.lights.push_back(sun2);

	Sprite sprite = Sprite("pappa.png", 0.5f);
	//world.sprites.push_back(sprite);

	GameObject floor = GameObject("plane", "tileFloor/colour.jpg", "tileFloor/normal.jpg", "tileFloor/colour.jpg");
	floor.transform.position.y = -1;
	floor.transform.scale = 10;
	floor.textureTiling = 2;
	floor.specularFactor = 10.0f;
	world.gameObjects.push_back(floor);

	GameObject room = GameObject("room", "white.png");
	room.transform.position.y = -1;
	room.textureTiling = 2.0f;
	world.gameObjects.push_back(room);

	GameObject sphere = GameObject("polySphere", "white.png");
	sphere.transform.position.y = 2;
	sphere.addReflection();
	world.gameObjects.push_back(sphere);

	GameObject barrel = GameObject("barrel", "barrel.png", "barrelNormal.png", "barrelSpecular.png");
	barrel.transform.scale = 0.2f;
	barrel.transform.position.x = 3;
	barrel.transform.position.z = 3;
	world.gameObjects.push_back(barrel);

	GameObject crate = GameObject("cube", "crate.png", "crateNormal.png");
	crate.transform.position = glm::vec3(-3, 0, 3);
	world.gameObjects.push_back(crate);

	GameObject monkey = GameObject("monkey", "white.png");
	monkey.specularFactor = 3.0f;
	monkey.transform.position = glm::vec3(-3, 0, -3);
	world.gameObjects.push_back(monkey);

	GameObject cone = GameObject("cube", "bricks.png", "bricksNormal.png");
	cone.transform.position = glm::vec3(3, 0, -3);
	world.gameObjects.push_back(cone);

	engine.bakeShadows(world);
	engine.bakeReflections(world);

	Logger::err("Had to skip reflection garbage collection!");

	camera.pitch = 20;

	while (engine.shouldRun())
	{
		engine.prepare();

		if (glfwGetKey(engine.display.window, GLFW_KEY_ENTER)) releaseControl = true;

		if (releaseControl) camera.update();
		else glfwSetInputMode(engine.display.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		if (!releaseControl)
		{
			cameraAngle += 0.5f * Engine::deltaTime;
			camera.position.x = distance * std::sin(cameraAngle) * std::cos(distance);
			camera.position.z = distance * std::cos(cameraAngle) * std::sin(distance);

			camera.yaw = -glm::degrees(cameraAngle) - 180;
			camera.pitch = 20;
		}

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