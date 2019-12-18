#include "Display.h"
#include <iostream>

#include <glad/glad.h>

std::vector<void(*)(GLFWwindow*, int, int)> Display::windowFunctions;

Display::Display(Config config)
{
	// Initialise OpenGL
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, config.glMajorVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, config.glMinorVersion);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create window
	window = glfwCreateWindow(config.width, config.height, config.windowName.c_str(), NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}
	else
	{
		std::cout << "Initialised GLAD with OpenGL " << config.glMajorVersion << "." << config.glMinorVersion << std::endl;
	}

}

void Display::subscribeToWindowChange(void processWindow(GLFWwindow*, int, int))
{
	windowFunctions.push_back(processWindow);
}

void Display::framebuffer_size_callback(GLFWwindow* _window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);

	for (auto p : windowFunctions)
		p(_window, width, height);
}


void Display::update()
{
	// Swap buffers and poll events
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void Display::beginFrame()
{
	for (auto p : inputFunctions)
		p(window);
}

void Display::subscribeToInput(void function(GLFWwindow*))
{
	inputFunctions.push_back(function);
}

bool Display::windowShouldClose()
{
	return glfwWindowShouldClose(window);
}

Display::~Display()
{
	// Dellocate GLFW resources
	glfwTerminate();
}
