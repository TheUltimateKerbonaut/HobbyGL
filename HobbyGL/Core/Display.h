#pragma once

#ifndef DISPLAY_H
#define DISPLAY_H

#include <GLFW\glfw3.h>

#include <vector>

#include "Config.h"


class Display
{


public:
	Display(Config config);
	~Display();

	void update();
	void subscribeToInput(void processInput(GLFWwindow*, int key, int scancode, int action, int mods));
	void subscribeToWindowChange(void processWindow(GLFWwindow*, int, int));
	void beginFrame();
	bool windowShouldClose();

	GLFWwindow* window;

private:
	static std::vector<void(*)(GLFWwindow*, int, int, int, int)> inputFunctions;
	static std::vector<void(*)(GLFWwindow*, int, int)> windowFunctions;

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	static void keyInput(GLFWwindow* window, int key, int scancode, int action, int mods);

};

#endif


