#include "DeferredLightingRenderer.h"
#include "../../Core/Camera.h"

#include <glad/glad.h>

#include <iostream>

DeferredLightingRenderer::DeferredLightingRenderer() : RenderShaderProgram("deferredLightingShaderVertex.glsl", "deferredLightingShaderFragment.glsl")
{
	getAllUniformLocations();
	connectTextureUnits();
}

void DeferredLightingRenderer::connectTextureUnits()
{
	this->loadInt(location_gPosition, 0);
	this->loadInt(location_gNormal, 1);
	this->loadInt(location_gColour, 2);
}

void DeferredLightingRenderer::getAllUniformLocations()
{
	location_gPosition = this->getUniformLocation("gPosition");
	location_gNormal = this->getUniformLocation("gNormal");
	location_gColour = this->getUniformLocation("gColour");

	location_directionals = this->getUniformLocation("directionals");
	location_points = this->getUniformLocation("points");
	for (unsigned int i = 0; i < maxLights; i++)
	{
		location_directionalColour[i] = this->getUniformLocation("directionalColour[" + std::to_string(i) + "]");
		location_directionalPos[i] = this->getUniformLocation("directionalPos[" + std::to_string(i) + "]");
		location_pointColour[i] = this->getUniformLocation("pointColour[" + std::to_string(i) + "]");
		location_pointPos[i] = this->getUniformLocation("pointPos[" + std::to_string(i) + "]");
		location_pointAttenuation[i] = this->getUniformLocation("pointAttenuation[" + std::to_string(i) + "]");
		location_pointRange[i] = this->getUniformLocation("pointRange[" + std::to_string(i) + "]");
	}
}

void DeferredLightingRenderer::bindAttributes()
{
	this->bindAttribute(0, "position");
	this->bindAttribute(1, "textureCoords");
}

void DeferredLightingRenderer::render(Sprite& sprite, Camera& camera, std::vector<std::reference_wrapper<Light>>& lights, unsigned int positionTexture, unsigned int normalTexture, unsigned int colourTexture)
{
	this->bind();
	this->connectTextureUnits();

	glBindVertexArray(sprite.mesh.vaoID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, positionTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normalTexture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, colourTexture);

	int directionals = 0;
	int points = 0;
	for (unsigned int i = 0; i < lights.size(); i++)
	{
		if (i >= maxLights)
			std::cerr << "Error: Too many lights - max supported 25" << std::endl;

		if (lights[i].get().lightType == Light::directional)
		{
			this->loadVec3(location_directionalColour[directionals], lights[i].get().colour);
			// Transform light position to view space!
			this->loadVec3(location_directionalPos[directionals], camera.viewMatrix * glm::vec4(lights[i].get().position.x, lights[i].get().position.y, lights[i].get().position.z, 1.0));
			directionals++;
		}
		else if (lights[i].get().lightType == Light::point)
		{
			this->loadVec3(location_pointColour[points], lights[i].get().colour);
			this->loadVec3(location_pointPos[points], camera.viewMatrix * glm::vec4(lights[i].get().position.x, lights[i].get().position.y, lights[i].get().position.z, 1.0));
			this->loadFloat(location_pointRange[points], lights[i].get().range);
			this->loadVec3(location_pointAttenuation[points], lights[i].get().attenuation);
			points++;
		}
	}
	this->loadInt(location_directionals, directionals);
	this->loadInt(location_points, points);

	glDrawElements(GL_TRIANGLES, sprite.mesh.vertexCount, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindVertexArray(0);

	this->unbind();
}

DeferredLightingRenderer::~DeferredLightingRenderer()
{
}
