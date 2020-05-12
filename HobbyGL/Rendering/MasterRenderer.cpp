#include "MasterRenderer.h"

#include <glad/glad.h>

#include "../Core/Engine.h"

bool MasterRenderer::sizeChanged;

MasterRenderer::MasterRenderer(Display& display) : renderImage(Loader::loadToVao(vertices, indices, textureCoords), Texture(), Transform()), 
gBufferRenderer(display), ssaoRenderer(display), ssaoBlurRenderer(display), hdrRenderer(display), bloomRenderer(display), ditheringRenderer(display), dofRenderer(display),
chromaticAbberation(display)
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
	glCullFace(GL_FRONT);
	for (unsigned int i = 0; i < world.lights.size(); ++i)
	{
		if (world.lights[i].get().lightType == Light::directional && world.lights[i].get().shadows)
		{
			glViewport(0, 0, Light::width, Light::height);
			shadowRenderer.bindFBO(world.lights[i].get().lightCount);
			world.lights[i].get().updateLightSpaceMatrix();
			prepareFrame(config);
			for (GameObject g : world.gameObjects)
			{
				shadowRenderer.render(g, world.lights[i].get());
			}
			shadowRenderer.unbindFBO();
		}
		else if (world.lights[i].get().lightType == Light::point && world.lights[i].get().shadows)
		{
			glViewport(0, 0, Light::pointWidth, Light::pointHeight);
			for (unsigned int face = 0; face < 6; ++face)
			{
				pointShadowRenderer.bindFBO(world.lights[i].get().lightCount, face);
				world.lights[i].get().updateLightSpaceMatrix();
				prepareFrame(config);
				for (GameObject g : world.gameObjects)
				{
					pointShadowRenderer.render(g, world.lights[i].get(), face, world.camera);
				}
				pointShadowRenderer.unbindFBO();
			}
		}
	}
	glCullFace(GL_BACK);
}

void MasterRenderer::bakeReflections(World& world, Config& config)
{
	glViewport(0, 0, Reflection::width, Reflection::height);

	int oldWidth = config.width;
	int oldHeight = config.height;
	float oldScale = config.resolutionScale;
	config.width = Reflection::width;
	config.height = Reflection::height;
	config.resolutionScale = 1;

	GBufferRenderer::sizeHasChanged = true;
	BloomRenderer::sizeHasChanged = true;
	DitheringRenderer::sizeHasChanged = true;
	DOFRenderer::sizeHasChanged = true;
	HDRRenderer::sizeHasChanged = true;
	SSAOBlurRenderer::sizeHasChanged = true;
	SSAORenderer::sizeHasChanged = true;
	ChromaticAbberation::sizeHasChanged = true;

	sizeChanged = true;
	

	for (GameObject g : world.gameObjects)
	{
		if (g.hasReflection)
		{
			for (unsigned int face = 0; face < 6; ++face)
			{
				glEnable(GL_DEPTH_TEST);
				glEnable(GL_CULL_FACE);
				glCullFace(GL_BACK);

				g.reflection.updateMatrices(g.transform.position, face);
				Camera camera = Camera(g.reflection.viewProjectionMatrix);
				camera.viewProjectionMatrix = g.reflection.viewProjectionMatrix;
				camera.viewMatrix = g.reflection.viewMatrix;
				camera.projectionMatrix = g.reflection.projectionMatrix;

				//camera.orthographicMatrix = g.reflection.orthographicMatrix;
				//camera.viewOrthographicMatrix = g.reflection.viewOrthographicMatrix;

				
				// G-Buffer pass
				gBufferRenderer.bindFBO();
				prepareFrame(config);

				for (GameObject x : world.gameObjects)
				{
					if (g.reflection.fbo != x.reflection.fbo) // Avoid self reflection
						gBufferRenderer.render(x, camera);
				}
				gBufferRenderer.unbindFBO();
				// I have to render twice lest the entire thing fails... ON ONLY ONE CUBEMAP FACE! Driver bug?
				gBufferRenderer.bindFBO();
				prepareFrame(config);
				for (GameObject x : world.gameObjects)
				{
					if (g.reflection.fbo != x.reflection.fbo)
						gBufferRenderer.render(x, camera);
				}
				gBufferRenderer.unbindFBO();

				// 2D rendering
				glDisable(GL_CULL_FACE);

				
				// SSAO pass
				ssaoRenderer.bindFBO();
				prepareFrame(config);
				ssaoRenderer.render(renderImage, camera, gBufferRenderer.gPosition, gBufferRenderer.gNormal);
				ssaoRenderer.unbindFBO();

				
				// SSAO Blur pass
				ssaoBlurRenderer.bindFBO();
				prepareFrame(config);
				ssaoBlurRenderer.render(renderImage, ssaoRenderer.ssaoColorBuffer);
				ssaoBlurRenderer.unbindFBO();
				

				// Deferred rendering pass
				g.reflection.bindFBO(face);
				prepareFrame(config);
				deferredLightingRenderer.render(renderImage, camera, world.lights, gBufferRenderer.gPosition, gBufferRenderer.gNormal, gBufferRenderer.gColorSpec,
					ssaoBlurRenderer.fboBlurTexture, shadowRenderer.shadowmapTexture, pointShadowRenderer.shadowmapTexture);
				g.reflection.unbindFBO();

				
			}
		}
	}

	config.width = oldWidth;
	config.height = oldHeight;
	config.resolutionScale = oldScale;

	glViewport(0, 0, (unsigned int)(config.width / config.resolutionScale), (unsigned int)(config.height / config.resolutionScale));

	GBufferRenderer::sizeHasChanged = true;
	BloomRenderer::sizeHasChanged = true;
	DitheringRenderer::sizeHasChanged = true;
	DOFRenderer::sizeHasChanged = true;
	HDRRenderer::sizeHasChanged = true;
	SSAOBlurRenderer::sizeHasChanged = true;
	SSAORenderer::sizeHasChanged = true;
	ChromaticAbberation::sizeHasChanged = true;

	sizeChanged = true;
}

void MasterRenderer::renderFrame(World& world, Config& config)
{
	//bakeReflections(world, config);
	//return;
	
	world.camera.updateViewMatrix();

	if (lastFrameResolutionScale != config.resolutionScale)
	{
		GBufferRenderer::sizeHasChanged = true;
		BloomRenderer::sizeHasChanged = true;
		DitheringRenderer::sizeHasChanged = true;
		DOFRenderer::sizeHasChanged = true;
		HDRRenderer::sizeHasChanged = true;
		SSAOBlurRenderer::sizeHasChanged = true;
		SSAORenderer::sizeHasChanged = true;
		ChromaticAbberation::sizeHasChanged = true;
		sizeChanged = true;
	}

	if (sizeChanged)
	{
		glDeleteTextures(1, &fboTexture);
		glDeleteBuffers(1, &fbo);
		constructFBO();
		sizeChanged = false;
	}

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
	deferredLightingRenderer.render(renderImage, world.camera, world.lights, gBufferRenderer.gPosition, gBufferRenderer.gNormal, gBufferRenderer.gColorSpec, 
		ssaoBlurRenderer.fboBlurTexture, shadowRenderer.shadowmapTexture, pointShadowRenderer.shadowmapTexture);
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
		hdrRenderer.render(renderImage, hdrRenderer.fboTexture, bloomRenderer.fboTextures[!horizontal]);
		ditheringRenderer.unbindFBO();

		// HDR pass
		bindFBO();
		prepareFrame(config);
		ditheringRenderer.render(renderImage, ditheringRenderer.fboTexture);
		unbindFBO();
	}
	else
	{
		// HDR pass
		bindFBO();
		prepareFrame(config);
		hdrRenderer.render(renderImage, hdrRenderer.fboTexture, bloomRenderer.fboTextures[!horizontal]);
		unbindFBO();
	}
	

	if (config.chromaticAbberation)
	{
		chromaticAbberation.bindFBO();
		prepareFrame(config);
		chromaticAbberation.render(renderImage, fboTexture);
		chromaticAbberation.unbindFBO();
	}

	// Switch back to native res
	glViewport(0, 0, config.width, config.height);

	// Enable 2D rendering
	prepareFrame(config);
	glDisable(GL_CULL_FACE);

	// Upscale result
	renderImage.texture.textureID = (config.chromaticAbberation) ? chromaticAbberation.fboTexture : fboTexture;
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


