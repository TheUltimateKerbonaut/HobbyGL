#pragma once	

#include "../../RenderShaderProgram.h"
#include "../../../../Core/Sprite.h"
#include "../../../../Core/Display.h"

#include <glad/glad.h>

#include <map>
#include <vector>

class ChromaticAbberation : RenderShaderProgram
{
public:
	ChromaticAbberation(Display& display);
	~ChromaticAbberation();

	void render(Sprite& sprite, unsigned int texture);

	virtual void connectTextureUnits();

	void bindFBO();
	void unbindFBO();

	unsigned int fboTexture;

	static bool sizeHasChanged;

private:

	static void onSizeChange(GLFWwindow* window, int width, int height);

	void constructFBO();

	virtual void getAllUniformLocations();
	virtual void bindAttributes();

	unsigned int location_texture;

	unsigned int fbo;

};

