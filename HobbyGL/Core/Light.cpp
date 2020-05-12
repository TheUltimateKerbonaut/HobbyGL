#include "Light.h"

#include <glad/glad.h>
#include <iostream>

float Light::near_plane = 0.01f;
float Light::far_plane = 32.0f;

int Light::width = 4096;
int Light::height = 4096;
int Light::pointWidth = 1500;
int Light::pointHeight = 1500;

unsigned int Light::currentDirectionalsLightCount = 0;
unsigned int Light::currentPointsLightCount = 0;

void Light::updateLightSpaceMatrix()
{
	if (lightType == Light::directional)
		lightProjection = glm::ortho(-shadowSize.x, shadowSize.x, -shadowSize.y, shadowSize.y, near_plane, far_plane);
	else
		lightProjection = glm::perspective(glm::radians(90.0f), (float)pointWidth / (float)pointHeight, near_plane, far_plane);

	if (lightType == Light::directional)
	{
		lightView = glm::lookAt(position,
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));
		lightSpaceMatrix = lightProjection * lightView;
	}
	else
	{
		lightViews[0] = glm::lookAt(position, position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));
		lightViews[1] = glm::lookAt(position, position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));
		lightViews[2] = glm::lookAt(position, position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
		lightViews[3] = glm::lookAt(position, position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0));
		lightViews[4] = glm::lookAt(position, position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0));
		lightViews[5] = glm::lookAt(position, position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0));

		lightSpaceMatrices[0] = lightProjection * lightViews[0];
		lightSpaceMatrices[1] = lightProjection * lightViews[1];
		lightSpaceMatrices[2] = lightProjection * lightViews[2];
		lightSpaceMatrices[3] = lightProjection * lightViews[3];
		lightSpaceMatrices[4] = lightProjection * lightViews[4];
		lightSpaceMatrices[5] = lightProjection * lightViews[5];

		lightSpaceMatrices[5] = lightProjection  * lightViews[5];
	}

	
}
