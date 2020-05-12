#include "PointShadowRenderer.h"
#include "../../Core/Camera.h"

#include "../../Core/Engine.h"

#include <glad/glad.h>

#include "../../../Utils/Logger.h"

PointShadowRenderer::PointShadowRenderer() : RenderShaderProgram("pointShadowShaderVertex.glsl", "pointShadowShaderFragment.glsl")
{
	getAllUniformLocations();

	// Create depth texture Sampler2DArrays
	unsigned int width = Light::pointWidth;
	unsigned int height = Light::pointHeight;
	unsigned int layerCount = DeferredLightingRenderer::maxLights;
	unsigned int mipLevelCount = 1;

	glGenTextures(1, &shadowmapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, shadowmapTexture);
	glTexImage3D(GL_TEXTURE_CUBE_MAP_ARRAY, 0, GL_DEPTH_COMPONENT, width, height, layerCount*6, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_BORDER_COLOR, borderColor);


	for (unsigned int i = 0; i < layerCount; ++i)
	{
		for (unsigned int face = 0; face < 6; ++face)
		{
			// Create FBO
			glGenFramebuffers(1, &shadowmapFBO[i*6 + face]);
			bindFBO(i, face);

			//glBindTexture(GL_TEXTURE_2D_ARRAY, shadowmapTexture);
			glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowmapTexture, 0, i*6 + face);


			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);

			unsigned int fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			{
				switch (fboStatus) {
				case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
					Logger::err(i + " GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
					break;
				case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
					Logger::err(i + " GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
					break;

				case GL_FRAMEBUFFER_UNSUPPORTED:
					Logger::err(i + " GL_FRAMEBUFFER_UNSUPPORTED");
					break;
				}
			}

			unbindFBO();
		}
		
	}

}

void PointShadowRenderer::bindFBO(unsigned int index, unsigned int face)
{
	//glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowmapTexture, 0, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowmapFBO[index * 6 + face]);
}

void PointShadowRenderer::unbindFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void PointShadowRenderer::connectTextureUnits()
{
	
}

void PointShadowRenderer::getAllUniformLocations()
{
	location_MVP = this->getUniformLocation("MVP");
	location_modelMatrix = this->getUniformLocation("modelMatrix");
	location_lightPos = this->getUniformLocation("lightPos");
	location_farPlane = this->getUniformLocation("farPlane");
}

void PointShadowRenderer::bindAttributes()
{
	this->bindAttribute(0, "position");
}


void PointShadowRenderer::render(GameObject& object, Light& light, unsigned int face, Camera& camera)
{
	this->bind();
	this->connectTextureUnits();

	glBindVertexArray(object.mesh.vaoID);
	glEnableVertexAttribArray(0);

	this->loadMat4(location_MVP, light.lightSpaceMatrices[face] * object.transform.getMatrix());
	this->loadMat4(location_modelMatrix, object.transform.getMatrix());
	this->loadVec3(location_lightPos, light.position);
	this->loadFloat(location_farPlane, Light::far_plane);

	glDrawElements(GL_TRIANGLES, object.mesh.vertexCount, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glBindVertexArray(0);

	this->unbind();
}


PointShadowRenderer::~PointShadowRenderer()
{
	glDeleteTextures(1, &shadowmapTexture);
	unsigned int layerCount = DeferredLightingRenderer::maxLights;
	for (unsigned int i = 0; i < layerCount; ++i) glDeleteFramebuffers(1, &shadowmapFBO[i]);
}
