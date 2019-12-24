#pragma once	

#include "../RenderShaderProgram.h"
#include "../DeferredLightingRenderer/DeferredLightingRenderer.h"
#include "../../Core/GameObject.h"
#include "../../Core/Light.h"

#include <glad/glad.h>

#include <map>
#include <vector>

#include "../../../Core/Light.h"

class ShadowRenderer : RenderShaderProgram
{
public:
	ShadowRenderer();
	~ShadowRenderer();

	void render(GameObject& sprite, Light& light);

	virtual void connectTextureUnits();

	void bindFBO(unsigned int index);
	void unbindFBO();

	unsigned int shadowmapFBO[15];
	unsigned int shadowmapTexture;

private:

	virtual void getAllUniformLocations();
	virtual void bindAttributes();

	unsigned int location_MVP;

};

