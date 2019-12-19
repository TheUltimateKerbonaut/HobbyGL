#include "DeferredLightingRenderer.h"
#include "../../Core/Camera.h"

#include <glad/glad.h>

#include <iostream>

DeferredLightingRenderer::DeferredLightingRenderer() : RenderShaderProgram("deferredLightingShader.vs", "deferredLightingShader.fs")
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
	
	location_viewPos = this->getUniformLocation("viewPos");
}

void DeferredLightingRenderer::bindAttributes()
{
	this->bindAttribute(0, "position");
	this->bindAttribute(1, "textureCoords");
}

void DeferredLightingRenderer::render(Sprite& sprite, Camera& camera, unsigned int positionTexture, unsigned int normalTexture, unsigned int colourTexture)
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
