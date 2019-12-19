#pragma once	

#include "../RenderShaderProgram.h"
#include "../../Core/Sprite.h"
#include "../../Core/Camera.h"

#include <map>
#include <vector>

class DeferredLightingRenderer : RenderShaderProgram
{
public:
	DeferredLightingRenderer();
	~DeferredLightingRenderer();

	void render(Sprite& sprite, Camera& camera, unsigned int positionTexture, unsigned int normalTexture, unsigned int colourTexture);

	virtual void connectTextureUnits();

private:
	virtual void getAllUniformLocations();
	virtual void bindAttributes();

	unsigned int location_gPosition;
	unsigned int location_gNormal;
	unsigned int location_gColour;

	unsigned int location_viewPos;

};

