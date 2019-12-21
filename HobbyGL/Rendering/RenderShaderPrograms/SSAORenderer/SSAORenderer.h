#pragma once	

#include "../RenderShaderProgram.h"
#include "../../Core/Sprite.h"
#include "../../Core/Camera.h"
#include "../../Core/Display.h"

#include <GLFW/glfw3.h>

#include <map>
#include <vector>

class SSAORenderer : RenderShaderProgram
{
public:
	SSAORenderer(Display& display);
	~SSAORenderer();

	void render(Sprite& sprite, Camera& camera, unsigned int gPosition, unsigned int gNormal);

	virtual void connectTextureUnits();

	void bindFBO();
	void unbindFBO();

	unsigned int ssaoFBO;
	unsigned int ssaoColorBuffer;

private:
	virtual void getAllUniformLocations();
	virtual void bindAttributes();

	std::vector<glm::vec3> ssaoKernel;
	std::vector<glm::vec3> ssaoNoise;

	unsigned int noiseTexture;

	unsigned int location_noiseTexture;
	unsigned int location_gPositionTexture;
	unsigned int location_gNormalTexture;

	unsigned int location_samples[64];
	unsigned int location_projectionMatrix;
	unsigned int location_noiseScale;

	void generateFBO();
	void generateSamples();

	static void onSizeChange(GLFWwindow* window, int width, int height);
	static bool sizeHasChanged;

};

