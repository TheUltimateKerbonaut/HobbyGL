#include "BloomRenderer.h"
#include "../../Core/Camera.h"

#include "../../Core/Engine.h"

#include <glad/glad.h>

bool BloomRenderer::sizeHasChanged;

BloomRenderer::BloomRenderer(Display& display) : RenderShaderProgram("BloomShaderVertex.glsl", "BloomShaderFragment.glsl")
{
	getAllUniformLocations();
	constructFBO();
	display.subscribeToWindowChange(onSizeChange);
}

void BloomRenderer::constructFBO()
{
	glGenFramebuffers(2, fbos);
	glGenTextures(2, fboTextures);
	for (int i = 0; i < 2; ++i)
	{
		bindFBO(i);

		// Color + specular color buffer
		glBindTexture(GL_TEXTURE_2D, fboTextures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, (unsigned int)(Engine::config.width / Engine::config.resolutionScale / scale), (unsigned int)(Engine::config.height / Engine::config.resolutionScale / scale), 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTextures[i], 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cerr << "Error: GBuffer frame buffer incomplete" << std::endl;

		unbindFBO();
	}
	
}

void BloomRenderer::connectTextureUnits()
{
	this->loadInt(location_texture, 0);
}

void BloomRenderer::getAllUniformLocations()
{
	location_texture = this->getUniformLocation("hdrTexture");
	location_horizontal = this->getUniformLocation("horizontal");
}

void BloomRenderer::bindAttributes()
{
	this->bindAttribute(0, "position");
	this->bindAttribute(1, "textureCoords");
}

void BloomRenderer::bindFBO(unsigned int index)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbos[index]);
}

void BloomRenderer::render(Sprite& object, unsigned int texture, bool horizontal)
{
	if (sizeHasChanged)
	{
		glDeleteTextures(1, &fboTextures[0]);
		glDeleteTextures(1, &fboTextures[1]);
		glDeleteFramebuffers(1, &fbos[0]);
		glDeleteFramebuffers(1, &fbos[1]);
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

	this->loadInt(location_horizontal, horizontal);

	glDrawElements(GL_TRIANGLES, object.mesh.vertexCount, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindVertexArray(0);

	this->unbind();
}

void BloomRenderer::unbindFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void BloomRenderer::onSizeChange(GLFWwindow* window, int width, int height)
{
	sizeHasChanged = true;
}

BloomRenderer::~BloomRenderer()
{
	glDeleteTextures(1, &fboTextures[0]);
	glDeleteTextures(1, &fboTextures[1]);
	glDeleteFramebuffers(1, &fbos[0]);
	glDeleteFramebuffers(1, &fbos[1]);
}
