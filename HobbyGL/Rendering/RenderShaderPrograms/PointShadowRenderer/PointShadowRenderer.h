#pragma once	

#include "../RenderShaderProgram.h"
#include "../DeferredLightingRenderer/DeferredLightingRenderer.h"
#include "../../Core/GameObject.h"
#include "../../Core/Light.h"
#include "../../Core/Camera.h"

#include <glad/glad.h>

#include <map>
#include <vector>

#include "../../../Core/Light.h"

class PointShadowRenderer : RenderShaderProgram
{
public:
	PointShadowRenderer();
	~PointShadowRenderer();

	void render(GameObject& sprite, Light& light, unsigned int face, Camera& camera);

	virtual void connectTextureUnits();

	void bindFBO(unsigned int index, unsigned int face);
	void unbindFBO();

	unsigned int shadowmapFBO[15*6];
	unsigned int shadowmapTexture;

private:

	virtual void getAllUniformLocations();
	virtual void bindAttributes();

	unsigned int location_MVP;
	unsigned int location_modelMatrix;
	unsigned int location_lightPos;
	unsigned int location_farPlane;

};

