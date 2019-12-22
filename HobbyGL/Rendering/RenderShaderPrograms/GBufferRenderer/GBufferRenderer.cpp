#include "GBufferRenderer.h"
#include "../../Core/Camera.h"

#include "../../Core/Engine.h"

#include <glad/glad.h>

bool GBufferRenderer::sizeHasChanged;

GBufferRenderer::GBufferRenderer(Display& display) : RenderShaderProgram("gBufferShaderVertex.glsl", "gBufferShaderFragment.glsl")
{
	getAllUniformLocations();
	constructFBO();
	display.subscribeToWindowChange(onSizeChange);
}

void GBufferRenderer::constructFBO()
{
	glGenFramebuffers(1, &gBuffer);
	bindFBO();

	// Position color buffer
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, (unsigned int)(Engine::config.width / Engine::config.resolutionScale), (unsigned int)(Engine::config.height / Engine::config.resolutionScale), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

	// Normal color buffer
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, (unsigned int)(Engine::config.width / Engine::config.resolutionScale), (unsigned int)(Engine::config.height / Engine::config.resolutionScale), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

	// Color + specular color buffer
	glGenTextures(1, &gColorSpec);
	glBindTexture(GL_TEXTURE_2D, gColorSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (unsigned int)(Engine::config.width / Engine::config.resolutionScale), (unsigned int)(Engine::config.height / Engine::config.resolutionScale), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gColorSpec, 0);

	// Tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	// Make renderbuffer object for depth buffer
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, (unsigned int)(Engine::config.width / Engine::config.resolutionScale), (unsigned int)(Engine::config.height / Engine::config.resolutionScale));
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cerr << "Error: GBuffer frame buffer incomplete" << std::endl;

	unbindFBO();
}

void GBufferRenderer::connectTextureUnits()
{
	this->loadInt(location_texture, 0);
}

void GBufferRenderer::getAllUniformLocations()
{
	location_texture = this->getUniformLocation("diffuseTexture");
	location_modelMatrix = this->getUniformLocation("modelMatrix");
	location_viewMatrix = this->getUniformLocation("viewMatrix");
	location_projectionMatrix = this->getUniformLocation("projectionMatrix");
	location_specularFactor = this->getUniformLocation("specularFactor");
}

void GBufferRenderer::bindAttributes()
{
	this->bindAttribute(0, "position");
	this->bindAttribute(1, "normal");
	this->bindAttribute(2, "textureCoords");
}

void GBufferRenderer::bindFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
}

void GBufferRenderer::render(GameObject& object, Camera& camera)
{
	if (sizeHasChanged)
	{
		glDeleteTextures(1, &gPosition);
		glDeleteTextures(1, &gNormal);
		glDeleteTextures(1, &gColorSpec);
		glDeleteFramebuffers(1, &gBuffer);
		constructFBO();
		sizeHasChanged = false;
	}

	this->bind();
	this->connectTextureUnits();

	glBindVertexArray(object.mesh.vaoID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, object.texture.textureID);

	this->loadMat4(location_modelMatrix, object.transform.getMatrix());
	this->loadMat4(location_viewMatrix, camera.viewMatrix);
	this->loadMat4(location_projectionMatrix, camera.projectionMatrix);

	this->loadFloat(location_specularFactor, object.specularFactor);

	glDrawElements(GL_TRIANGLES, object.mesh.vertexCount, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindVertexArray(0);

	this->unbind();
}

void GBufferRenderer::unbindFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GBufferRenderer::onSizeChange(GLFWwindow* window, int width, int height)
{
	sizeHasChanged = true;
}

GBufferRenderer::~GBufferRenderer()
{
	glDeleteFramebuffers(1, &gBuffer);
}
