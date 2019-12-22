#include "MasterRenderer.h"

#include <glad/glad.h>

#include "../Core/Engine.h"

MasterRenderer::MasterRenderer(Display& display) : renderImage(Loader::loadToVao(vertices, indices, textureCoords), Texture(), Transform()), 
gBufferRenderer(display), ssaoRenderer(display), hdrRenderer(display), bloomRenderer(display)
{

}

void MasterRenderer::prepareFrame(Config& config)
{
	glClearColor(config.backgroundColourR, config.backgroundColourG, config.backgroundColourB, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MasterRenderer::renderFrame(World& world, Config& config)
{
	world.camera.updateViewMatrix();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
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

	// SSAO pass
	ssaoRenderer.bindFBO();
	prepareFrame(config);
	glDisable(GL_CULL_FACE);
	ssaoRenderer.render(renderImage, world.camera, gBufferRenderer.gPosition, gBufferRenderer.gNormal);
	ssaoRenderer.unbindFBO();

	// SSAO Blur pass
	ssaoBlurRenderer.bindFBO();
	prepareFrame(config);
	glDisable(GL_CULL_FACE);
	ssaoBlurRenderer.render(renderImage, ssaoRenderer.ssaoColorBuffer);
	ssaoBlurRenderer.unbindFBO();

	// Deferred rendering pass
	hdrRenderer.bindFBO();
	prepareFrame(config);
	glDisable(GL_CULL_FACE);
	deferredLightingRenderer.render(renderImage, world.camera, world.lights, gBufferRenderer.gPosition, gBufferRenderer.gNormal, gBufferRenderer.gColorSpec, ssaoBlurRenderer.fboBlurTexture);
	hdrRenderer.unbindFBO();

	// Bloom pass
	bool horizontal = true, first_iteration = true;
	unsigned int amount = 10;
	glViewport(0, 0, (unsigned int)(config.width / config.resolutionScale / bloomRenderer.scale), (unsigned int)(config.height / config.resolutionScale / bloomRenderer.scale));
	for (unsigned int i = 0; i < amount; i++)
	{
		bloomRenderer.bindFBO(horizontal);
		bloomRenderer.render(renderImage, (first_iteration) ? hdrRenderer.fboBrightTexture : bloomRenderer.fboTextures[!horizontal], horizontal);
		horizontal = !horizontal;
		if (first_iteration)
			first_iteration = false;
	}
	bloomRenderer.unbindFBO();
	glViewport(0, 0, (unsigned int)(config.width / config.resolutionScale), (unsigned int)(config.height / config.resolutionScale));

	// Enable 2D rendering
	prepareFrame(config);
	glDisable(GL_CULL_FACE);

	// HDR pass
	hdrRenderer.render(renderImage, hdrRenderer.fboTexture, bloomRenderer.fboTextures[!horizontal]);

	// Switch back to native res
	glViewport(0, 0, config.width, config.height);

	for (Sprite s : world.sprites)
	{
		//s.texture.textureID = bloomRenderer.fboTextures[1];
		spriteRenderer.render(s, world.camera);
	}

}

MasterRenderer::~MasterRenderer()
{
}


