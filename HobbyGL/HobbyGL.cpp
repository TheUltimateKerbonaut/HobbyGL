// HobbyGL.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <glad/glad.h>
#include <GLFW\glfw3.h>

#include <iostream>


#include "Config.h"
#include "Display.h"
#include "Loader.h"
#include "MasterRenderer.h"
#include "World.h"
#include "Engine.h"

#include "Sprite.h"

#include "Texture.h"

#include "Transform.h"

static void quitWhenEscape(GLFWwindow* window);

int main()
{

	Engine engine = Engine();

	engine.display.subscribeToInput(quitWhenEscape);

	World world = World();

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
	Sprite cube = Sprite(Loader::loadToVao(vertices, indices, textureCoords),
		Loader::loadTexture("crateNormal.png"),
		Transform());
	cube.transform.scale = 0.3;

	cube.mesh = Loader::loadToVao(vertices, indices, textureCoords);
	world.sprites.push_back(cube);

	while (engine.shouldRun())
	{	
		engine.prepare();
		
		cube.transform.scale += 0.01f;
		cube.transform.rotY += 3.0f;
		cube.transform.x += 0.001f;

		engine.update(world);
	}

	return 0;
}


static void quitWhenEscape(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}