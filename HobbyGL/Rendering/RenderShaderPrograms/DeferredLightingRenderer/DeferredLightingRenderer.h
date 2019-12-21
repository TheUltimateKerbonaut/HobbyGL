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

	void render(Sprite& sprite, Camera& camera, std::vector<std::reference_wrapper<Light>>& lights, unsigned int positionTexture, unsigned int normalTexture, unsigned int colourTexture, unsigned int ssaoColorBuffer);

	virtual void connectTextureUnits();

private:
	virtual void getAllUniformLocations();
	virtual void bindAttributes();

	unsigned int location_gPosition;
	unsigned int location_gNormal;
	unsigned int location_gColour;
	unsigned int location_ssao;

	const unsigned int maxLights = 25;
	unsigned int location_directionalColour[25];
	unsigned int location_directionalPos[25];
	unsigned int location_directionals;

	unsigned int location_pointColour[25];
	unsigned int location_pointPos[25];
	unsigned int location_pointAttenuation[25];
	unsigned int location_pointRange[25];
	unsigned int location_points;

};

