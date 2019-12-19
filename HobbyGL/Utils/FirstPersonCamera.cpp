#include "FirstPersonCamera.h"

#include <GLFW/glfw3.h>


FirstPersonCamera::FirstPersonCamera(Display& display) : Camera(display)
{
	window = display.window;
	showMouse = false;
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void FirstPersonCamera::update()
{
	float moveSpeed = 0.05f;


	float forward = 0.0f;
	float right = 0.0f;

	bool doesYaw = false;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		forward += moveSpeed;

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		forward -= moveSpeed;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		forward -= moveSpeed;
		doesYaw = true;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		forward += moveSpeed;
		doesYaw = true;
	}

	float finalForward = cos(glm::radians(yaw)) * forward;
	float finalRight = sin(glm::radians(yaw)) * forward;

	if (doesYaw)
		position += glm::vec3(finalForward, 0, finalRight);
	else
		position += glm::vec3(finalRight, 0, -finalForward);

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		position += up * moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		position -= up * moveSpeed;

	if (glfwGetKey(window, GLFW_KEY_F1))
		showMouse = !showMouse;

	glfwSetInputMode(window, GLFW_CURSOR, (showMouse) ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);

	if (!showMouse)
	{
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);
		mouse_callback(window, xPos, yPos);
	}

}

void FirstPersonCamera::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

	if (firstFrame)
	{
		lastX = (float) xpos;
		lastY = (float)ypos;
		firstFrame = false;
	}

	float xoffset = (float)xpos - (float)lastX;
	float yoffset = (float)lastY - (float)ypos;
	lastX = (float)xpos;
	lastY = (float)ypos;

	float sensitivity = 0.10f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch -= yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;


}

FirstPersonCamera::~FirstPersonCamera()
{

}
