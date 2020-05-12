#include "ChromaticAbberation.h"
#include "../../Core/Camera.h"

#include "../../Core/Engine.h"

#include <glad/glad.h>

#include "../../../../Utils/Logger.h"

bool ChromaticAbberation::sizeHasChanged;

ChromaticAbberation::ChromaticAbberation(Display& display) : RenderShaderProgram("chromaticAberrationVertex.glsl", "chromaticAberrationFragment.glsl")
{
	getAllUniformLocations();
	constructFBO();
	display.subscribeToWindowChange(onSizeChange);
}

void ChromaticAbberation::constructFBO()
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
		Logger::err("Error: Dithering frame buffer incomplete");

	unbindFBO();
	
}

void ChromaticAbberation::connectTextureUnits()
{
	this->loadInt(location_texture, 0);
}

void ChromaticAbberation::getAllUniformLocations()
{
	location_texture = this->getUniformLocation("inputTexture");
}

void ChromaticAbberation::bindAttributes()
{
	this->bindAttribute(0, "position");
	this->bindAttribute(1, "textureCoords");
}

void ChromaticAbberation::bindFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void ChromaticAbberation::render(Sprite& object, unsigned int texture)
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

	glDrawElements(GL_TRIANGLES, object.mesh.vertexCount, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindVertexArray(0);

	this->unbind();
}

void ChromaticAbberation::unbindFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ChromaticAbberation::onSizeChange(GLFWwindow* window, int width, int height)
{
	sizeHasChanged = true;
}

ChromaticAbberation::~ChromaticAbberation()
{
	glDeleteTextures(1, &fboTexture);
	glDeleteFramebuffers(1, &fbo);
}
