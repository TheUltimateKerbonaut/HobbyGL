#pragma once

#include "RenderShaderProgram.h"
#include "Sprite.h"

class SpriteRenderer : RenderShaderProgram
{
public:
	SpriteRenderer();
	~SpriteRenderer();

	void render(Sprite& sprite);

	virtual void connectTextureUnits();

private:
	virtual void getAllUniformLocations();
	virtual void bindAttributes();

	unsigned int location_texture;
	unsigned int location_MVP;

};

