#pragma once

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Display.h"

class Camera
{
public:
	Camera(Display& display);
	~Camera();

	glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
	float pitch = 0;
	float yaw = 0;
	float roll = 0;

	glm::mat4 projectionMatrix;
	glm::mat4 orthographicMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 viewProjectionMatrix;
	glm::mat4 viewOrthographicMatrix;

	void updateProjectionMatrix();
	void updateViewMatrix();

protected:
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

private:
	float FOV = 70.0f;
	float zNEAR = 0.01f;
	float zFAR = 1000.0f;

	static bool needsToChange;

	static void onDisplaySizeChange(GLFWwindow* window, int width, int height);

};

