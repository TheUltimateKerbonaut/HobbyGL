#pragma once	

#include "../RenderShaderProgram.h"
#include "../../Core/GameObject.h"
#include "../../Core/Camera.h"

#include <map>
#include <vector>

class GBufferRenderer : RenderShaderProgram
{
public:
	GBufferRenderer();
	~GBufferRenderer();

	void render(GameObject& sprite, Camera& camera);

	virtual void connectTextureUnits();

private:
	virtual void getAllUniformLocations();
	virtual void bindAttributes();

	unsigned int location_texture;
	unsigned int location_MVP;

};

