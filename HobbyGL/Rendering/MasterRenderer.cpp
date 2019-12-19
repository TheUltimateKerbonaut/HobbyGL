#include "MasterRenderer.h"

#include <glad/glad.h>

MasterRenderer::MasterRenderer()
{
}

void MasterRenderer::prepareFrame(Config& config)
{
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glClearColor(config.backgroundColourR, config.backgroundColourG, config.backgroundColourB, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MasterRenderer::renderFrame(World& world, Config& config)
{
	world.camera.updateViewMatrix();

	for (GameObject g : world.gameObjects)
	{
		gBufferRenderer.render(g, world.camera);
	}

	for (Sprite s : world.sprites)
	{
		spriteRenderer.render(s, world.camera);
	}

}

MasterRenderer::~MasterRenderer()
{
}


