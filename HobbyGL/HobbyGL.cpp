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

static void quitWhenEscape(GLFWwindow* window);

int main()
{

	Engine engine = Engine();

	engine.display.subscribeToInput(quitWhenEscape);

	Camera camera = Camera(engine.display);
	World world = World(camera);

	std::vector<float> vertices = {
	 0.5f,  0.5f, 0.0f,  // top right
	 0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f,  // bottom left
	-0.5f,  0.5f, 0.0f   // top left 
	};
	std::vector<unsigned int> indices = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};
	std::vector<float> textureCoords = {
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f
	};

	for (int i = -10; i < 10; i++)
	{
		for (int x = -10; x < 10; x++)
		{
			Sprite *cube = new Sprite(Loader::loadToVao(vertices, indices, textureCoords),
				Loader::loadTexture("crateNormal.png"),
				Transform());
			cube->transform.scale = 0.3;

			cube->transform.x = 0.3 * i;
			cube->transform.y = 0.3 * x;

			cube->mesh = Loader::loadToVao(vertices, indices, textureCoords);
			world.sprites.push_back(*cube);
		}
	}
	

	while (engine.shouldRun())
	{	
		engine.prepare();
		
		/*cube.transform.scale += 0.01f;
		cube.transform.rotY += 3.0f;
		cube.transform.x += 0.001f;*/
		
		camera.roll += 0.9f;
		//camera.position.z -= 0.005f;

		engine.update(world);
	}

	return 0;
}


static void quitWhenEscape(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}