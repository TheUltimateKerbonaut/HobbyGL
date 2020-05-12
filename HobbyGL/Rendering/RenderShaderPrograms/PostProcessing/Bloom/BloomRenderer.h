#pragma once	

#include "../../RenderShaderProgram.h"
#include "../../../../Core/Sprite.h"
#include "../../../../Core/Display.h"

#include <glad/glad.h>

#include <map>
#include <vector>

class BloomRenderer : RenderShaderProgram
{
public:
	BloomRenderer(Display& display);
	~BloomRenderer();

	void render(Sprite& sprite, unsigned int texture, bool horizontal);

	virtual void connectTextureUnits();

	void bindFBO(unsigned int index);
	void unbindFBO();

	unsigned int fboTextures[2];

	const float scale = 1;

	static bool sizeHasChanged;

private:

	static void onSizeChange(GLFWwindow* window, int width, int height);

	void constructFBO();

	virtual void getAllUniformLocations();
	virtual void bindAttributes();

	unsigned int location_texture;
	unsigned int location_horizontal;

	unsigned int fbos[2];

};

