#include "MasterRenderer.h"

#include <glad/glad.h>

#include "../Core/Engine.h"

bool MasterRenderer::sizeChanged;

MasterRenderer::MasterRenderer(Display& display) : renderImage(Loader::loadToVao(vertices, indices, textureCoords), Texture(), Transform()), 
gBufferRenderer(display), ssaoRenderer(display), hdrRenderer(display), bloomRenderer(display), ditheringRenderer(display)
{
	constructFBO();
	display.subscribeToWindowChange(sizeDidChange);
}

void MasterRenderer::bindFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void MasterRenderer::unbindFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void MasterRenderer::constructFBO()
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
		std::cerr << "Error: GBuffer frame buffer incomplete" << std::endl;

	unbindFBO();

}

void MasterRenderer::sizeDidChange(GLFWwindow* window, int width, int height)
{
	sizeChanged = true;
}

void MasterRenderer::prepareFrame(Config& config)
{
	glClearColor(config.backgroundColourR, config.backgroundColourG, config.backgroundColourB, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MasterRenderer::renderFrame(World& world, Config& config)
{

	if (sizeChanged)
	{
		glDeleteTextures(1, &fboTexture);
		glDeleteBuffers(1, &fbo);
		constructFBO();
		sizeChanged = false;
	}

	world.camera.updateViewMatrix();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// Shadows passess
	glViewport(0, 0, Light::width, Light::height);
	glCullFace(GL_FRONT);
	for (unsigned int i = 0; i < world.lights.size(); ++i)
	{
		if (world.lights[i].get().lightType == Light::directional && world.lights[i].get().shadows)
		{
			shadowRenderer.bindFBO(world.lights[i].get().lightCount);
			world.lights[i].get().updateLightSpaceMatrix();
			prepareFrame(config);
			for (GameObject g : world.gameObjects)
			{
				shadowRenderer.render(g, world.lights[i].get());
			}
			shadowRenderer.unbindFBO();
		}
	}
	glCullFace(GL_BACK);

	// Switch to scaled res
	glViewport(0, 0, (unsigned int)(config.width / config.resolutionScale), (unsigned int)(config.height / config.resolutionScale));

	// G-Buffer pass
	gBufferRenderer.bindFBO();
	prepareFrame(config);
	for (GameObject g : world.gameObjects)
	{
		gBufferRenderer.render(g, world.camera);
	}
	gBufferRenderer.unbindFBO();

	// 2D rendering
	glDisable(GL_CULL_FACE);

	// SSAO pass
	ssaoRenderer.bindFBO();
	prepareFrame(config);
	ssaoRenderer.render(renderImage, world.camera, gBufferRenderer.gPosition, gBufferRenderer.gNormal);
	ssaoRenderer.unbindFBO();

	// SSAO Blur pass
	ssaoBlurRenderer.bindFBO();
	prepareFrame(config);
	ssaoBlurRenderer.render(renderImage, ssaoRenderer.ssaoColorBuffer);
	ssaoBlurRenderer.unbindFBO();

	// Deferred rendering pass
	hdrRenderer.bindFBO();
	prepareFrame(config);
	deferredLightingRenderer.render(renderImage, world.camera, world.lights, gBufferRenderer.gPosition, gBufferRenderer.gNormal, gBufferRenderer.gColorSpec, ssaoBlurRenderer.fboBlurTexture, shadowRenderer.shadowmapTexture);
	hdrRenderer.unbindFBO();

	// Bloom pass
	bool horizontal = true, first_iteration = true;
	if (config.bloom)
	{
		unsigned int amount = 10;
		glViewport(0, 0, (unsigned int)(config.width / config.resolutionScale / bloomRenderer.scale), (unsigned int)(config.height / config.resolutionScale / bloomRenderer.scale));
		for (unsigned int i = 0; i < amount; i++)
		{
			bloomRenderer.bindFBO(horizontal);
			prepareFrame(config);
			bloomRenderer.render(renderImage, (first_iteration) ? hdrRenderer.fboBrightTexture : bloomRenderer.fboTextures[!horizontal], horizontal);
			horizontal = !horizontal;
			if (first_iteration)
				first_iteration = false;
		}
		bloomRenderer.unbindFBO();
		glViewport(0, 0, (unsigned int)(config.width / config.resolutionScale), (unsigned int)(config.height / config.resolutionScale));
	}
	else
	{
		bloomRenderer.bindFBO(!horizontal);
		prepareFrame(config);
		bloomRenderer.unbindFBO();
	}

	// Dithering pass
	if (config.dithering)
	{
		ditheringRenderer.bindFBO();
		prepareFrame(config);
		ditheringRenderer.render(renderImage, hdrRenderer.fboTexture);
		ditheringRenderer.unbindFBO();
	}

	// HDR pass
	bindFBO();
	prepareFrame(config);
	hdrRenderer.render(renderImage, config.dithering ? ditheringRenderer.fboTexture : hdrRenderer.fboTexture, bloomRenderer.fboTextures[!horizontal]);
	unbindFBO();

	// Switch back to native res
	glViewport(0, 0, config.width, config.height);

	// Enable 2D rendering
	prepareFrame(config);
	glDisable(GL_CULL_FACE);

	// Upscale result
	renderImage.texture.textureID = fboTexture;
	spriteRenderer.render(renderImage);

	for (Sprite s : world.sprites)
	{
		spriteRenderer.render(s, world.camera);
	}

}

MasterRenderer::~MasterRenderer()
{
	glDeleteTextures(1, &fboTexture);
	glDeleteBuffers(1, &fbo);
}


