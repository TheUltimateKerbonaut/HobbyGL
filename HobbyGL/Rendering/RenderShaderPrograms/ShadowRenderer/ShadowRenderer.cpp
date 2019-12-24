#include "ShadowRenderer.h"
#include "../../Core/Camera.h"

#include "../../Core/Engine.h"

#include <glad/glad.h>

ShadowRenderer::ShadowRenderer() : RenderShaderProgram("shadowShaderVertex.glsl", "shadowShaderFragment.glsl")
{
	getAllUniformLocations();

	// Create depth texture Sampler2DArrays
	unsigned int width = Light::width;
	unsigned int height = Light::height;
	unsigned int layerCount = DeferredLightingRenderer::maxLights;
	unsigned int mipLevelCount = 1;

	glGenTextures(1, &shadowmapTexture);
	glBindTexture(GL_TEXTURE_2D_ARRAY, shadowmapTexture);
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT, width, height, layerCount, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	//float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	

	for (unsigned int i = 0; i < layerCount; ++i)
	{
		// Create FBO
		glGenFramebuffers(1, &shadowmapFBO[i]);
		bindFBO(i);

		//glBindTexture(GL_TEXTURE_2D_ARRAY, shadowmapTexture);
		glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowmapTexture, 0, i);
		
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		unsigned int fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			switch (fboStatus) {
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
				std::cerr << i << " GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT" << std::endl;
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
				std::cerr << i << " GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT" << std::endl;
				break;

			case GL_FRAMEBUFFER_UNSUPPORTED:
				std::cerr << i << " GL_FRAMEBUFFER_UNSUPPORTED" << std::endl;
				break;
			}
		}

		unbindFBO();
		//glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
	}

}

void ShadowRenderer::bindFBO(unsigned int index)
{
	glBindFramebuffer(GL_FRAMEBUFFER, shadowmapFBO[index]);
}

void ShadowRenderer::unbindFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void ShadowRenderer::connectTextureUnits()
{
	
}

void ShadowRenderer::getAllUniformLocations()
{
	location_MVP = this->getUniformLocation("MVP");
}

void ShadowRenderer::bindAttributes()
{
	this->bindAttribute(0, "position");
}


void ShadowRenderer::render(GameObject& object, Light& light)
{
	this->bind();
	this->connectTextureUnits();

	glBindVertexArray(object.mesh.vaoID);
	glEnableVertexAttribArray(0);

	this->loadMat4(location_MVP, light.lightSpaceMatrix * object.transform.getMatrix());

	glDrawElements(GL_TRIANGLES, object.mesh.vertexCount, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glBindVertexArray(0);

	this->unbind();
}


ShadowRenderer::~ShadowRenderer()
{
	glDeleteTextures(1, &shadowmapTexture);
	unsigned int layerCount = DeferredLightingRenderer::maxLights;
	for (unsigned int i = 0; i < layerCount; ++i) glDeleteFramebuffers(1, &shadowmapFBO[i]);
}
