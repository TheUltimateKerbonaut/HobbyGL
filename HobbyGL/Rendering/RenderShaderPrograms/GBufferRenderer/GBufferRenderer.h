#pragma once	

#include "../RenderShaderProgram.h"
#include "../../Core/GameObject.h"
#include "../../Core/Camera.h"
#include "../../Core/Display.h"

#include <glad/glad.h>

#include <map>
#include <vector>

class GBufferRenderer : RenderShaderProgram
{
public:
	GBufferRenderer(Display& display);
	~GBufferRenderer();

	void render(GameObject& sprite, Camera& camera);

	virtual void connectTextureUnits();

	void bindFBO();
	void unbindFBO();

	unsigned int gPosition, gNormal, gColorSpec;

private:

	static bool sizeHasChanged;
	static void onSizeChange(GLFWwindow* window, int width, int height);

	void constructFBO();

	virtual void getAllUniformLocations();
	virtual void bindAttributes();

	unsigned int location_texture;
	unsigned int location_modelMatrix;
	unsigned int location_viewMatrix;
	unsigned int location_projectionMatrix;
	unsigned int location_specularFactor;

	unsigned int location_normalMap;
	unsigned int location_hasNormalMap;

	unsigned int gBuffer;

};

