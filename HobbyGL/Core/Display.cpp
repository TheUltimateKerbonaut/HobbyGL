#include "Display.h"
#include <iostream>

#include <glad/glad.h>

#include "Engine.h"

std::vector<void(*)(GLFWwindow*, int, int)> Display::windowFunctions;
std::vector<void(*)(GLFWwindow*, int, int, int, int)> Display::inputFunctions;

void centerWindow(GLFWwindow *window, GLFWmonitor *monitor)
{
	if (!monitor)
		return;

	const GLFWvidmode *mode = glfwGetVideoMode(monitor);
	if (!mode)
		return;

	int monitorX, monitorY;
	glfwGetMonitorPos(monitor, &monitorX, &monitorY);

	int windowWidth, windowHeight;
	glfwGetWindowSize(window, &windowWidth, &windowHeight);

	glfwSetWindowPos(window,
		monitorX + (mode->width - windowWidth) / 2,
		monitorY + (mode->height - windowHeight) / 2);
}

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

	int monitorCount;
	GLFWmonitor **monitors = glfwGetMonitors(&monitorCount);
	centerWindow(window, monitors[0]);
	
	glfwSetKeyCallback(window, keyInput);
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
	glfwSwapInterval(Engine::config.vsync);
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void Display::beginFrame()
{
}

void Display::keyInput(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	for (auto p : inputFunctions)
		p(window, key, scancode, action, mods);
}

void Display::subscribeToInput(void function(GLFWwindow*, int key, int scancode, int action, int mods))
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
