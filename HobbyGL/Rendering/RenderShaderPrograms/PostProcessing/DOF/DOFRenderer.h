#pragma once	

#include "../../RenderShaderProgram.h"
#include "../../../../Core/Sprite.h"
#include "../../../../Core/Display.h"
#include "../../../../Core/Camera.h"

#include <glad/glad.h>

#include <map>
#include <vector>

class DOFRenderer : RenderShaderProgram
{
public:
	DOFRenderer(Display& display);
	~DOFRenderer();

	void render(Sprite& sprite, unsigned int depthTexture, unsigned int blurTexture, unsigned int norrmalTexture, Camera& camera);

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

	unsigned int location_depthTexture;
	unsigned int location_blurTexture;
	unsigned int location_normalTexture;

	unsigned int location_zNear;
	unsigned int location_zFar;
	unsigned int location_focusDistance;
	unsigned int location_focusRange;
	unsigned int location_apeture;
	unsigned int location_focalLength;

	unsigned int fbo;

};

