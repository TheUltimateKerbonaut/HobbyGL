#pragma once	

#include "../RenderShaderProgram.h"
#include "../../Core/Sprite.h"
#include "../../Core/Camera.h"
#include "../../Core/Display.h"
#include "../../Core/Light.h"

#include <glfw/glfw3.h>

#include <map>
#include <vector>

class DeferredLightingRenderer : RenderShaderProgram
{

public:
	DeferredLightingRenderer();
	~DeferredLightingRenderer();

	void render(Sprite& sprite, Camera& camera, std::vector<std::reference_wrapper<Light>>& lights, unsigned int positionTexture, unsigned int normalTexture, unsigned int colourTexture, unsigned int ssaoColorBuffer, unsigned int directionalShadowmap);

	virtual void connectTextureUnits();

	const static unsigned int maxLights = 15;

private:
	virtual void getAllUniformLocations();
	virtual void bindAttributes();

	unsigned int location_gPosition;
	unsigned int location_gNormal;
	unsigned int location_gColour;
	unsigned int location_ssao;

	unsigned int location_directionalColour[15];
	unsigned int location_directionalPos[15];
	unsigned int location_directionals;
	unsigned int location_directionalShadows[15];
	unsigned int location_directionalLightSpaceMatrices[15];
	unsigned int location_directionalShadowmaps;

	unsigned int location_pointColour[15];
	unsigned int location_pointPos[15];
	unsigned int location_pointAttenuation[15];
	unsigned int location_pointRange[15];
	unsigned int location_points;

};

