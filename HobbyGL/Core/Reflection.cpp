#include "Reflection.h"

#include "../Utils/Logger.h"
#include "../Rendering/Loader.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

int Reflection::width = 512;
int Reflection::height = 512;
float Reflection::nearPlane = 0.01f;
float Reflection::farPlane = 100.0f;

Reflection::Reflection()
{

}

Reflection::Reflection(glm::vec3 _position)
{
	constructFBO();
}

void Reflection::constructFBO()
{
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glGenTextures(1, &fboTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, fboTexture);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	for (unsigned int i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
			width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

	// Attach for completeness
	for (unsigned int i = 0; i < 6; ++i)
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, fboTexture, 0);

	unsigned int fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		switch (fboStatus) {
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			Logger::err("Reflection GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			Logger::err("Reflection GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED:
			Logger::err("Reflection GL_FRAMEBUFFER_UNSUPPORTED");
			break;
		}
	}


	unbindFBO();

	//std::string files[6] = { "skybox/right.png", "skybox/left.png", "skybox/up.png", "skybox/down.png", "skybox/front.png", "skybox/back.png" };
	//fboTexture = Loader::loadCubemapTexture(files).textureID;

}

void Reflection::bindFBO(unsigned int face)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, fboTexture, 0);
}

void Reflection::unbindFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Reflection::updateMatrices(glm::vec3 position, int face)
{
	if (face == 0) viewMatrix = glm::lookAt(position, position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));
	if (face == 1) viewMatrix = glm::lookAt(position, position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));
	if (face == 2) viewMatrix = glm::lookAt(position, position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
	if (face == 3) viewMatrix = glm::lookAt(position, position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0));
	if (face == 4) viewMatrix = glm::lookAt(position, position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0));
	if (face == 5) viewMatrix = glm::lookAt(position, position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0));

	projectionMatrix = glm::perspective(glm::radians(90.0f), (float)width / (float)height, nearPlane, farPlane);

	viewProjectionMatrix = projectionMatrix * viewMatrix;

	/*viewMatrix = glm::lookAt(position, position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0));
	projectionMatrix = glm::perspective(glm::radians(90.0f), (float)width / (float)height, nearPlane, farPlane);
	viewProjectionMatrix = projectionMatrix * viewMatrix;*/

	orthographicMatrix = glm::ortho(0.0f, (float)width, (float)height, 0.0f, nearPlane, farPlane);
	viewOrthographicMatrix = orthographicMatrix * viewMatrix;
}

void Reflection::destroyFBO()
{
	//glDeleteTextures(1, &fboTexture);
	//glDeleteFramebuffers(6, &fbo[0]);
}

Reflection::~Reflection()
{

}