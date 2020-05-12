#pragma once

#include "..//Core/World.h"
#include "../Core/Config.h"
#include "../Core/Mesh.h"
#include "../Core/Sprite.h"
#include "../Core/Display.h"

#include "RenderShaderPrograms/SpriteRenderer/SpriteRenderer.h"
#include "RenderShaderPrograms/GBufferRenderer/GBufferRenderer.h"
#include "RenderShaderPrograms/DeferredLightingRenderer/deferredLightingRenderer.h"
#include "RenderShaderPrograms/SSAORenderer/SSAORenderer.h"
#include "RenderShaderPrograms/SSAOBlurRenderer/SSAOBlurRenderer.h"
#include "RenderShaderPrograms/ShadowRenderer/ShadowRenderer.h"
#include "RenderShaderPrograms/PointShadowRenderer/PointShadowRenderer.h"
#include "RenderShaderPrograms/PostProcessing/HDR/HDRRenderer.h"
#include "RenderShaderPrograms/PostProcessing/Bloom/BloomRenderer.h"
#include "RenderShaderPrograms/PostProcessing/Dithering/DitheringRenderer.h"
#include "RenderShaderPrograms/PostProcessing/DOF/DOFRenderer.h"
#include "RenderShaderPrograms/PostProcessing/ChromaticAbberation/ChromaticAbberation.h"

#include <GLFW/glfw3.h>

#include <map>
#include <vector>

class MasterRenderer
{
public:
	MasterRenderer(Display& display);
	~MasterRenderer();

	void prepareFrame(Config& config);
	void renderFrame(World& world, Config& config);

private:
	SpriteRenderer spriteRenderer;
	GBufferRenderer gBufferRenderer;
	DeferredLightingRenderer deferredLightingRenderer;
	SSAORenderer ssaoRenderer;
	SSAOBlurRenderer ssaoBlurRenderer;
	ShadowRenderer shadowRenderer;
	PointShadowRenderer pointShadowRenderer;
	HDRRenderer hdrRenderer;
	BloomRenderer bloomRenderer;
	DitheringRenderer ditheringRenderer;
	DOFRenderer dofRenderer;
	ChromaticAbberation chromaticAbberation;

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
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f
	};

	Sprite renderImage;

	unsigned int fbo;
	unsigned int fboTexture;

	void constructFBO();
	void bindFBO();
	void unbindFBO();

	static bool sizeChanged;
	static void sizeDidChange(GLFWwindow* window, int width, int height);

};

