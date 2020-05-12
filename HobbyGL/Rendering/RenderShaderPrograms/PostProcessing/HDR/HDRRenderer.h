#pragma once	

#include "../../RenderShaderProgram.h"
#include "../../../../Core/Sprite.h"
#include "../../../../Core/Display.h"

#include <glad/glad.h>

#include <map>
#include <vector>

class HDRRenderer : RenderShaderProgram
{
public:
	HDRRenderer(Display& display);
	~HDRRenderer();

	void render(Sprite& sprite, unsigned int texture, unsigned int bloomTexture);

	virtual void connectTextureUnits();

	void bindFBO();
	void unbindFBO();

	unsigned int fboTexture;
	unsigned int fboBrightTexture;

	static bool sizeHasChanged;

private:

	static void onSizeChange(GLFWwindow* window, int width, int height);

	void constructFBO();

	virtual void getAllUniformLocations();
	virtual void bindAttributes();

	unsigned int location_texture;
	unsigned int location_bloomTexture;

	unsigned int fbo;

};

