#include "MasterRenderer.h"

#include <glad/glad.h>

MasterRenderer::MasterRenderer() : renderImage(Loader::loadToVao(vertices, indices, textureCoords), Texture(), Transform())
{

}

void MasterRenderer::prepareFrame(Config& config)
{
	glClearColor(config.backgroundColourR, config.backgroundColourG, config.backgroundColourB, 1.0f);
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

	// Enable 2D rendering
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	// Deferred lighting pass
	deferredLightingRenderer.render(renderImage, world.camera, gBufferRenderer.gPosition, gBufferRenderer.gNormal, gBufferRenderer.gColorSpec);

	for (Sprite s : world.sprites)
	{
		spriteRenderer.render(s, world.camera);
	}

}

MasterRenderer::~MasterRenderer()
{
}


