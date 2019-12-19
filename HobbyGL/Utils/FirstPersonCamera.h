#pragma once

#include "../Core/Camera.h"
#include "../Core/Display.h"

#include <GLFW/glfw3.h>

class FirstPersonCamera : public Camera
{
public:
	FirstPersonCamera(Display& display);
	~FirstPersonCamera();

	void mouse_callback(GLFWwindow* window, double xpos, double ypos);

	void update();

private:
	bool showMouse;
	float lastX;
	float lastY;
	bool firstFrame;

	GLFWwindow* window;

};

