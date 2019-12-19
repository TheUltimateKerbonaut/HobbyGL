#include "Camera.h"
#include "Engine.h"

#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

bool Camera::needsToChange;

Camera::Camera(Display& display)
{
	//pitch = 90;

	updateProjectionMatrix();
	display.subscribeToWindowChange(onDisplaySizeChange);
}

void Camera::updateProjectionMatrix()
{
	projectionMatrix = glm::perspective(glm::radians((float)FOV), (float)Engine::config.width / (float)Engine::config.height, (float)zNEAR, (float)zFAR);
	orthographicMatrix = glm::ortho(0.0f, (float)Engine::config.width, (float)Engine::config.height, 0.0f, zNEAR, zFAR);
}

void Camera::updateViewMatrix()
{
	viewMatrix = glm::mat4(1.0f);
	viewMatrix *= glm::rotate((float)glm::radians(pitch), glm::vec3(1, 0, 0));
	viewMatrix *= glm::rotate((float)glm::radians(yaw), glm::vec3(0, 1, 0));
	viewMatrix *= glm::rotate((float)glm::radians(roll), glm::vec3(0, 0, 1));
	viewMatrix *= glm::translate(glm::vec3(-position.x, -position.y, -position.z));

	if (needsToChange)
	{
		updateProjectionMatrix();
		needsToChange = false;
	}

	viewProjectionMatrix = projectionMatrix * viewMatrix;
	viewOrthographicMatrix = orthographicMatrix * glm::translate(glm::vec3(0, 0, -1));
}

void Camera::onDisplaySizeChange(GLFWwindow* window, int width, int height)
{
	needsToChange = true;
}

Camera::~Camera()
{
}
