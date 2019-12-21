#pragma once	

#include "../RenderShaderProgram.h"
#include "../../Core/Sprite.h"
#include "../../Core/Camera.h"

#include <map>
#include <vector>

class SSAOBlurRenderer : RenderShaderProgram
{
public:
	SSAOBlurRenderer();
	~SSAOBlurRenderer();

	void render(Sprite& sprite, unsigned int texture);

	virtual void connectTextureUnits();

	unsigned int ssaoFBO;
	unsigned int fboBlurTexture;

	void bindFBO();
	void unbindFBO();

private:
	virtual void getAllUniformLocations();
	virtual void bindAttributes();

	unsigned int location_texture;

	void generateFBO();

	static void onSizeChange(GLFWwindow* window, int width, int height);
	static bool sizeHasChanged;

};

