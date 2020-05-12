#include "DOFRenderer.h"
#include "../../Core/Camera.h"

#include "../../Core/Engine.h"

#include <glad/glad.h>

#include "../../../../Utils/Logger.h"

bool DOFRenderer::sizeHasChanged;

DOFRenderer::DOFRenderer(Display& display) : RenderShaderProgram("DOFShaderVertex.glsl", "DOFShaderFragment.glsl")
{
	getAllUniformLocations();
	constructFBO();
	display.subscribeToWindowChange(onSizeChange);
}

void DOFRenderer::constructFBO()
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

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		Logger::err("Error: Depth of field frame buffer incomplete");

	unbindFBO();
	
}

void DOFRenderer::connectTextureUnits()
{
	this->loadInt(location_depthTexture, 0);
	this->loadInt(location_blurTexture, 1);
	this->loadInt(location_normalTexture, 2);
}

void DOFRenderer::getAllUniformLocations()
{
	location_depthTexture = this->getUniformLocation("depthTexture");
	location_blurTexture = this->getUniformLocation("blurTexture");
	location_normalTexture = this->getUniformLocation("normalTexture");

	location_zNear = this->getUniformLocation("zNear");
	location_zFar = this->getUniformLocation("zFar");

	location_focusDistance = this->getUniformLocation("focusDistance");
	location_focusRange = this->getUniformLocation("focusRange");
	location_apeture = this->getUniformLocation("apeture");
	location_focalLength = this->getUniformLocation("focalLength");
}

void DOFRenderer::bindAttributes()
{
	this->bindAttribute(0, "position");
	this->bindAttribute(1, "textureCoords");
}

void DOFRenderer::bindFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void DOFRenderer::render(Sprite& object, unsigned int depthTexture, unsigned int blurTexture, unsigned int norrmalTexture, Camera& camera)
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
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, blurTexture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, norrmalTexture);

	this->loadFloat(location_zNear, camera.zNEAR);
	this->loadFloat(location_zFar, camera.zFAR);

	this->loadFloat(location_focusDistance, camera.focusDistance);
	this->loadFloat(location_focusRange, camera.focusRange);
	this->loadFloat(location_apeture, camera.apeture);
	this->loadFloat(location_focalLength, camera.focalLength);

	glDrawElements(GL_TRIANGLES, object.mesh.vertexCount, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindVertexArray(0);

	this->unbind();
}

void DOFRenderer::unbindFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DOFRenderer::onSizeChange(GLFWwindow* window, int width, int height)
{
	sizeHasChanged = true;
}

DOFRenderer::~DOFRenderer()
{
	glDeleteTextures(1, &fboTexture);
	glDeleteFramebuffers(1, &fbo);
}
