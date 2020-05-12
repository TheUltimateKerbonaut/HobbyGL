#include "DeferredLightingRenderer.h"
#include "../../Core/Camera.h"

#include <glad/glad.h>

#include "../../../Utils/Logger.h"

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
	this->loadInt(location_ssao, 3);
	this->loadInt(location_directionalShadowmaps, 4);
	this->loadInt(location_pointShadowmaps, 5);
}

void DeferredLightingRenderer::getAllUniformLocations()
{
	location_gPosition = this->getUniformLocation("gPosition");
	location_gNormal = this->getUniformLocation("gNormal");
	location_gColour = this->getUniformLocation("gColour");
	location_ssao = this->getUniformLocation("ssao");
	location_directionalShadowmaps = this->getUniformLocation("directionalShadowmaps");
	location_pointShadowmaps = this->getUniformLocation("pointShadowmaps");

	location_viewMatrix = this->getUniformLocation("viewMatrixInverse");
	location_viewPos = this->getUniformLocation("viewPos");

	location_directionals = this->getUniformLocation("directionals");
	location_points = this->getUniformLocation("points");
	location_pointFarPlane = this->getUniformLocation("pointFarPlane");
	for (unsigned int i = 0; i < maxLights; i++)
	{
		location_directionalColour[i] = this->getUniformLocation("directionalColour[" + std::to_string(i) + "]");
		location_directionalPos[i] = this->getUniformLocation("directionalPos[" + std::to_string(i) + "]");
		location_directionalShadows[i] = this->getUniformLocation("directionalShadows[" + std::to_string(i) + "]");
		location_directionalLightSpaceMatrices[i] = this->getUniformLocation("directionalLightSpaceMatrices[" + std::to_string(i) + "]");

		location_pointColour[i] = this->getUniformLocation("pointColour[" + std::to_string(i) + "]");
		location_pointPos[i] = this->getUniformLocation("pointPos[" + std::to_string(i) + "]");
		location_pointAttenuation[i] = this->getUniformLocation("pointAttenuation[" + std::to_string(i) + "]");
		location_pointRange[i] = this->getUniformLocation("pointRange[" + std::to_string(i) + "]");
		location_pointShadows[i] = this->getUniformLocation("pointShadows[" + std::to_string(i) + "]");
	}
}

void DeferredLightingRenderer::bindAttributes()
{
	this->bindAttribute(0, "position");
	this->bindAttribute(1, "textureCoords");
}

void DeferredLightingRenderer::render(Sprite& sprite, Camera& camera, std::vector<std::reference_wrapper<Light>>& lights, unsigned int positionTexture, unsigned int normalTexture, unsigned int colourTexture,
	unsigned int ssaoColorBuffer, unsigned int directionalShadowmap, unsigned int pointShadowmap)
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
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D_ARRAY, directionalShadowmap);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, pointShadowmap);

	int directionals = 0;
	int points = 0;
	for (unsigned int i = 0; i < lights.size(); i++)
	{
		if (i >= maxLights)
			Logger::err("Error: Too many lights - max supported " + maxLights);

		if (lights[i].get().lightType == Light::directional)
		{
			this->loadVec3(location_directionalColour[directionals], lights[i].get().colour);
			// Transform light position to view space!
			this->loadVec3(location_directionalPos[directionals], camera.viewMatrix * glm::vec4(lights[i].get().position.x, lights[i].get().position.y, lights[i].get().position.z, 1.0));

			if (lights[i].get().shadows)
			{
				this->loadMat4(location_directionalLightSpaceMatrices[directionals], lights[i].get().lightSpaceMatrix * glm::inverse(camera.viewMatrix));
				this->loadInt(location_directionalShadows[directionals], lights[i].get().lightCount);
			}
			else this->loadInt(location_directionalShadows[directionals], -1);

			directionals++;
		}
		else if (lights[i].get().lightType == Light::point)
		{
			this->loadVec3(location_pointColour[points], lights[i].get().colour);
			this->loadVec3(location_pointPos[points], camera.viewMatrix * glm::vec4(lights[i].get().position.x, lights[i].get().position.y, lights[i].get().position.z, 1.0));
			this->loadFloat(location_pointRange[points], lights[i].get().range);
			this->loadVec3(location_pointAttenuation[points], lights[i].get().attenuation);
			this->loadInt(location_pointShadows[points], lights[i].get().shadows);
			points++;
		}
	}
	this->loadInt(location_directionals, directionals);
	this->loadInt(location_points, points);
	this->loadFloat(location_pointFarPlane, Light::far_plane);

	this->loadMat4(location_viewMatrix, glm::inverse(camera.viewMatrix));
	this->loadVec3(location_viewPos, camera.position);

	glDrawElements(GL_TRIANGLES, sprite.mesh.vertexCount, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindVertexArray(0);

	this->unbind();
}

DeferredLightingRenderer::~DeferredLightingRenderer()
{
}
