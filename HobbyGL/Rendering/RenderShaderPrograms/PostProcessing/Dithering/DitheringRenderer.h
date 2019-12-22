#pragma once	

#include "../../RenderShaderProgram.h"
#include "../../../../Core/Sprite.h"
#include "../../../../Core/Display.h"

#include <glad/glad.h>

#include <map>
#include <vector>

class DitheringRenderer : RenderShaderProgram
{
public:
	DitheringRenderer(Display& display);
	~DitheringRenderer();

	void render(Sprite& sprite, unsigned int texture);

	virtual void connectTextureUnits();

	void bindFBO();
	void unbindFBO();

	unsigned int fboTexture;

private:

	static bool sizeHasChanged;
	static void onSizeChange(GLFWwindow* window, int width, int height);

	void constructFBO();

	virtual void getAllUniformLocations();
	virtual void bindAttributes();

	unsigned int location_texture;

	unsigned int fbo;

};

