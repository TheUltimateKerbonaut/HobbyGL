#pragma once

#include "RenderShaderProgram.h"
#include "../../Core/Sprite.h"
#include "../../Core/Camera.h"

#include <map>
#include <vector>

class SpriteRenderer : RenderShaderProgram
{
public:
	SpriteRenderer();
	~SpriteRenderer();

	void render(Sprite& sprite, Camera& camera);

	virtual void connectTextureUnits();

private:
	virtual void getAllUniformLocations();
	virtual void bindAttributes();

	unsigned int location_texture;
	unsigned int location_MVP;

};

