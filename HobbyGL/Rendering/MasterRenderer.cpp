#include "MasterRenderer.h"

#include <glad/glad.h>

MasterRenderer::MasterRenderer(Display& display) : renderImage(Loader::loadToVao(vertices, indices, textureCoords), Texture(), Transform()), gBufferRenderer(display), ssaoRenderer(display)
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

	// Enable 2D rendering
	prepareFrame(config);
	glDisable(GL_CULL_FACE);

	// Deferred lighting pass
	deferredLightingRenderer.render(renderImage, world.camera, world.lights, gBufferRenderer.gPosition, gBufferRenderer.gNormal, gBufferRenderer.gColorSpec, ssaoRenderer.ssaoColorBuffer);

	for (Sprite s : world.sprites)
	{
		s.texture.textureID = ssaoRenderer.ssaoColorBuffer;
		spriteRenderer.render(s, world.camera);
	}

}

MasterRenderer::~MasterRenderer()
{
}


