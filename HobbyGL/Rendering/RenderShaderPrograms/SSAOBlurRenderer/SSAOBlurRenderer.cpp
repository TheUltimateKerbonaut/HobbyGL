#include "SSAOBlurRenderer.h"
#include "../../Core/Camera.h"
#include "../../Core/Engine.h"

#include <glad/glad.h>

#include <iostream>

#include "../../../Utils/Logger.h"

bool SSAOBlurRenderer::sizeHasChanged;

SSAOBlurRenderer::SSAOBlurRenderer(Display& display) : RenderShaderProgram("SSAOBlurShaderVertex.glsl", "SSAOBlurShaderFragment.glsl")
{
	getAllUniformLocations();
	connectTextureUnits();
	generateFBO();

	display.subscribeToWindowChange(onSizeChange);
}

void SSAOBlurRenderer::generateFBO()
{

	glGenFramebuffers(1, &ssaoFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
	glGenTextures(1, &fboBlurTexture);
	glBindTexture(GL_TEXTURE_2D, fboBlurTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, (unsigned int)(Engine::config.width / Engine::config.resolutionScale), (unsigned int)(Engine::config.height / Engine::config.resolutionScale), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboBlurTexture, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		Logger::err("Error: SSAO blur frame buffer incomplete");

	unbindFBO();
}

void SSAOBlurRenderer::connectTextureUnits()
{
	this->loadInt(location_texture, 0);
}

void SSAOBlurRenderer::getAllUniformLocations()
{
	location_texture = this->getUniformLocation("ssaoTexture");
}

void SSAOBlurRenderer::bindAttributes()
{
	this->bindAttribute(0, "position");
	this->bindAttribute(1, "textureCoords");
}

void SSAOBlurRenderer::render(Sprite& sprite, unsigned int texture)
{
	if (sizeHasChanged)
	{
		glDeleteTextures(1, &fboBlurTexture);
		glDeleteFramebuffers(1, &ssaoFBO);
		generateFBO();
		sizeHasChanged = false;
	}

	this->bind();
	this->connectTextureUnits();

	glBindVertexArray(sprite.mesh.vaoID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glDrawElements(GL_TRIANGLES, sprite.mesh.vertexCount, GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindVertexArray(0);

	this->unbind();
}

void SSAOBlurRenderer::onSizeChange(GLFWwindow* window, int width, int height)
{
	sizeHasChanged = true;
}

void SSAOBlurRenderer::bindFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
}

void SSAOBlurRenderer::unbindFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

SSAOBlurRenderer::~SSAOBlurRenderer()
{
	glDeleteTextures(1, &fboBlurTexture);
	glDeleteFramebuffers(1, &ssaoFBO);
}
