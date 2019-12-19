#pragma once

#include "..//Core/World.h"
#include "../Core/Config.h"
#include "../Core/Mesh.h"
#include "../Core/Sprite.h"

#include "RenderShaderPrograms/SpriteRenderer/SpriteRenderer.h"
#include "RenderShaderPrograms/GBufferRenderer/GBufferRenderer.h"
#include "RenderShaderPrograms/DeferredLightingRenderer/deferredLightingRenderer.h"

#include <map>
#include <vector>

class MasterRenderer
{
public:
	MasterRenderer();
	~MasterRenderer();

	void prepareFrame(Config& config);
	void renderFrame(World& world, Config& config);

private:
	SpriteRenderer spriteRenderer;
	GBufferRenderer gBufferRenderer;
	DeferredLightingRenderer deferredLightingRenderer;

	const std::vector<float> vertices = {
	 1.0f,  1.0f, 0.0f,  // top right
	 1.0f, -1.0f, 0.0f,  // bottom right
	-1.0f, -1.0f, 0.0f,  // bottom left
	-1.0f,  1.0f, 0.0f   // top left 
	};
	const std::vector<unsigned int> indices = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};
	const std::vector<float> textureCoords = {
		0.0f, 0.0f,
		0.0f, -1.0f,
		-1.0f, -1.0f,
		-1.0f, 0.0f
	};

	Sprite renderImage;

};

