#pragma once

#include <glm/glm.hpp>

#include <algorithm>

class Light
{
public:

	enum Type { directional, point };

	Light(Light::Type _type)
	{
		lightType = _type;
		colour = glm::vec3(1, 1, 1);
		calculateRadius();
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
		range = 1000.0f;
	}

};