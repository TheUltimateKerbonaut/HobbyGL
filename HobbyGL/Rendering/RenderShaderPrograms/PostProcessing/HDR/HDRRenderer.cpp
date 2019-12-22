#include "HDRRenderer.h"
#include "../../Core/Camera.h"

#include "../../Core/Engine.h"

#include <glad/glad.h>

bool HDRRenderer::sizeHasChanged;

HDRRenderer::HDRRenderer(Display& display) : RenderShaderProgram("HDRShaderVertex.glsl", "HDRShaderFragment.glsl")
{
	getAllUniformLocations();
	constructFBO();
	display.subscribeToWindowChange(onSizeChange);
}

void HDRRenderer::constructFBO()
{
	glGenFramebuffers(1, &fbo);
	bindFBO();

	// Colour buffer
	glGenTextures(1, &fboTexture);
	glBindTexture(GL_TEXTURE_2D, fboTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, (unsigned int)(Engine::config.width / Engine::config.resolutionScale), (unsigned int)(Engine::config.height / Engine::config.resolutionScale), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTexture, 0);

	// Bright buffer
	glGenTextures(1, &fboBrightTexture);
	glBindTexture(GL_TEXTURE_2D, fboBrightTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, (unsigned int)(Engine::config.width / Engine::config.resolutionScale), (unsigned int)(Engine::config.height / Engine::config.resolutionScale), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, fboBrightTexture, 0);

	// Attach both
	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cerr << "Error: GBuffer frame buffer incomplete" << std::endl;

	unbindFBO();
}

void HDRRenderer::connectTextureUnits()
{
	this->loadInt(location_texture, 0);
	this->loadInt(location_bloomTexture, 1);
}

void HDRRenderer::getAllUniformLocations()
{
	location_texture = this->getUniformLocation("hdrTexture");
	location_bloomTexture = this->getUniformLocation("bloomTexture");
}

void HDRRenderer::bindAttributes()
{
	this->bindAttribute(0, "position");
	this->bindAttribute(1, "textureCoords");
}

void HDRRenderer::bindFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void HDRRenderer::render(Sprite& object, unsigned int texture, unsigned int bloomTexture)
{
	if (sizeHasChanged)
	{
		glDeleteTextures(1, &fboTexture);
		glDeleteFramebuffers(1, &fbo);
		constructFBO();
		sizeHasChanged = false;
	}

	this->bind();
	this->connectTextureUnits();

	glBindVertexArray(object.mesh.vaoID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, bloomTexture);

	glDrawElements(GL_TRIANGLES, object.mesh.vertexCount, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindVertexArray(0);

	this->unbind();
}

void HDRRenderer::unbindFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void HDRRenderer::onSizeChange(GLFWwindow* window, int width, int height)
{
	sizeHasChanged = true;
}

HDRRenderer::~HDRRenderer()
{
	glDeleteTextures(1, &fboTexture);
	glDeleteFramebuffers(1, &fbo);
}
