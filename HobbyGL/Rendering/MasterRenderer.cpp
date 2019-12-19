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
		glBindVertexArray(g.mesh.vaoID);
		glEnableVertexAttribArray(0);
		glDrawElements(GL_TRIANGLES, g.mesh.vertexCount, GL_UNSIGNED_INT, 0);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
	}

	for (Sprite s : world.sprites)
	{
		spriteRenderer.render(s, world.camera);
	}

}

MasterRenderer::~MasterRenderer()
{
}


