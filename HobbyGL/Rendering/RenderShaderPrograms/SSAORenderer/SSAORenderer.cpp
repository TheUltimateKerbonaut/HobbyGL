#include "SSAORenderer.h"
#include "../../Core/Camera.h"
#include "../../Core/Engine.h"

#include <glad/glad.h>

#include <iostream>
#include <random>
#include <vector>

bool SSAORenderer::sizeHasChanged;

SSAORenderer::SSAORenderer(Display& display) : RenderShaderProgram("SSAOShaderVertex.glsl", "SSAOShaderFragment.glsl")
{
	getAllUniformLocations();
	connectTextureUnits();

	generateSamples();

	glGenTextures(1, &noiseTexture);
	glBindTexture(GL_TEXTURE_2D, noiseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);

	generateFBO();

	display.subscribeToWindowChange(onSizeChange);
}

void SSAORenderer::generateFBO()
{
	glGenFramebuffers(1, &ssaoFBO);
	bindFBO();

	glGenTextures(1, &ssaoColorBuffer);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, Engine::config.width, Engine::config.height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cerr << "Error: GBuffer frame buffer incomplete" << std::endl;

	unbindFBO();
}

float lerp(float a, float b, float f)
{
	return a + f * (b - a);
}

void SSAORenderer::generateSamples()
{
	std::uniform_real_distribution<float> randomFloats(0.0, 1.0); // random floats between 0.0 - 1.0
	std::default_random_engine generator;
	for (unsigned int i = 0; i < 64; ++i)
	{
		glm::vec3 sample(
			randomFloats(generator) * 2.0 - 1.0,
			randomFloats(generator) * 2.0 - 1.0,
			randomFloats(generator)
		);
		sample = glm::normalize(sample);
		sample *= randomFloats(generator);
		float scale = (float)i / 64.0f;
		scale = lerp(0.1f, 1.0f, scale * scale);
		sample *= scale;
		ssaoKernel.push_back(sample);
	}

	for (unsigned int i = 0; i < 16; i++)
	{
		glm::vec3 noise(
			randomFloats(generator) * 2.0 - 1.0,
			randomFloats(generator) * 2.0 - 1.0,
			0.0f);
		ssaoNoise.push_back(noise);
	}
}

void SSAORenderer::connectTextureUnits()
{
	this->loadInt(location_noiseTexture, 0);
	this->loadInt(location_gPositionTexture, 1);
	this->loadInt(location_gNormalTexture, 2);
}

void SSAORenderer::getAllUniformLocations()
{
	location_noiseTexture = this->getUniformLocation("noiseTexture");
	location_gPositionTexture = this->getUniformLocation("gPositionTexture");
	location_gNormalTexture = this->getUniformLocation("gNormalTexture");

	for (int i = 0; i < 64; i++) location_samples[i] = this->getUniformLocation("samples[" + std::to_string(i) + "]");
	location_projectionMatrix = this->getUniformLocation("projectionMatrix");
	location_noiseScale = this->getUniformLocation("noiseScale");
}

void SSAORenderer::bindAttributes()
{
	this->bindAttribute(0, "position");
	this->bindAttribute(1, "textureCoords");
}

void SSAORenderer::render(Sprite& sprite, Camera& camera, unsigned int gPosition, unsigned int gNormal)
{
	if (sizeHasChanged)
	{
		glDeleteTextures(1, &ssaoColorBuffer);
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
	glBindTexture(GL_TEXTURE_2D, noiseTexture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gPosition);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	
	for (unsigned int i = 0; i < ssaoKernel.size(); ++i) this->loadVec3(location_samples[i], ssaoKernel[i]);
	this->loadMat4(location_projectionMatrix, camera.projectionMatrix);
	this->loadVec2(location_noiseScale, glm::vec2((float)Engine::config.width / 4.0f, (float)Engine::config.height / 4.0f));

	glDrawElements(GL_TRIANGLES, sprite.mesh.vertexCount, GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindVertexArray(0);

	this->unbind();
}

void SSAORenderer::bindFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
}

void SSAORenderer::unbindFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SSAORenderer::onSizeChange(GLFWwindow* window, int width, int height)
{
	sizeHasChanged = true;
}

SSAORenderer::~SSAORenderer()
{
	glDeleteTextures(1, &ssaoColorBuffer);
	glDeleteFramebuffers(1, &ssaoFBO);
}
