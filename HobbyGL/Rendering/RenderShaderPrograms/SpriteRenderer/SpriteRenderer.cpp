#include "SpriteRenderer.h"
#include "../../Core/Camera.h"

#include <glad/glad.h>

#include <iostream>

SpriteRenderer::SpriteRenderer() : RenderShaderProgram("spriteShaderVertex.glsl", "spriteShaderFragment.glsl")
{
	getAllUniformLocations();
	connectTextureUnits();
}

void SpriteRenderer::connectTextureUnits()
{
	this->loadInt(location_texture, 0);
}

void SpriteRenderer::getAllUniformLocations()
{
	location_texture = this->getUniformLocation("diffuseTexture");
	location_MVP = this->getUniformLocation("MVP");
}

void SpriteRenderer::bindAttributes()
{
	this->bindAttribute(0, "position");
	this->bindAttribute(1, "textureCoords");
}

void SpriteRenderer::render(Sprite& sprite, Camera& camera)
{
	this->bind();
	this->connectTextureUnits();

	glBindVertexArray(sprite.mesh.vaoID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sprite.texture.textureID);
	glm::mat4 MVP = camera.viewOrthographicMatrix * sprite.transform.getMatrix();
	this->loadMat4(location_MVP, MVP);

	glDrawElements(GL_TRIANGLES, sprite.mesh.vertexCount, GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindVertexArray(0);

	this->unbind();
}

SpriteRenderer::~SpriteRenderer()
{
}
