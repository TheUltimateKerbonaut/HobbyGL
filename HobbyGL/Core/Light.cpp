#include "Light.h"

#include <glad/glad.h>
#include <iostream>

int Light::width = 4096;
int Light::height = 4096;
unsigned int Light::currentDirectionalsLightCount = 0;

void Light::updateLightSpaceMatrix()
{
	lightProjection = glm::ortho(-shadowSize.x, shadowSize.x, -shadowSize.y, shadowSize.y, near_plane, far_plane);
	lightView = glm::lookAt(position,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	lightSpaceMatrix = lightProjection * lightView;
}
