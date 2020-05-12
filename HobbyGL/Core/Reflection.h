#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Reflection
{

public:
	
	Reflection();
	Reflection(glm::vec3 position);
	~Reflection();

	void updateMatrices(glm::vec3 position, int face);
	void constructFBO();
	void destroyFBO();

	static int width;
	static int height;
	static float nearPlane;
	static float farPlane;

	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::mat4 viewProjectionMatrix;
	glm::mat4 orthographicMatrix;
	glm::mat4 viewOrthographicMatrix;

	unsigned int fbo;
	unsigned int fboTexture;

	void bindFBO(unsigned int face);
	void unbindFBO();

};

