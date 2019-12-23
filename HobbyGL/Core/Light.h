#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <algorithm>

#include <glad/glad.h>

class Light
{
public:

	enum Type { directional, point };

	Light(Light::Type _type, bool _shadows)
	{
		lightType = _type;
		colour = glm::vec3(1, 1, 1);
		calculateRadius();

		shadows = _shadows;
		if (_type == Light::directional)
		{
			lightCount = currentDirectionalsLightCount;
			currentDirectionalsLightCount++;
		}
	}
	~Light()
	{
		
	}

	Type lightType;

	glm::vec3 position;
	glm::vec3 colour;

	glm::vec3 attenuation = glm::vec3(1.0f, 0.14f, 0.07f);
	float range;

	inline void calculateRadius()
	{
		float constant = 1.0f;
		float linear = 0.7f;
		float quadratic = 1.8f;

		constant = attenuation.x;
		linear = attenuation.y;
		quadratic = attenuation.z;

		float lightMax = std::max(std::max(colour.x, colour.y), colour.z);
		float radius = (float)((-linear + std::sqrt(linear * linear - 4.0f * quadratic * (constant - (256.0f / 5.0f) * lightMax)))
			/ (2.0f * quadratic));

		range = radius;
	}

	// Shadows
	float near_plane = 1.0f, far_plane = 32.0f;
	glm::vec2 shadowSize = glm::vec2(20.0f, 20.0f);
	glm::mat4 lightProjection;
	glm::mat4 lightView;
	glm::mat4 lightSpaceMatrix;
	void updateLightSpaceMatrix();


	bool shadows = false;
	static int width;
	static int height;
	unsigned int lightCount;
	static unsigned int currentDirectionalsLightCount;

};