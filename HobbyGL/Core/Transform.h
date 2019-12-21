#pragma once

#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

class Transform
{
public:
	
	glm::vec3 position;

	float scale;

	glm::vec3 rotation;

	inline Transform()
	{
		position.x = 0;
		position.y = 0;
		position.z = 0;
		rotation.x = 0;
		rotation.y = 0;
		rotation.z = 0;
		scale = 1;
	}

	inline glm::mat4 getMatrix()
	{
		glm::mat4 translationMatrix = glm::translate(position);

		glm::mat4 _rotX = glm::rotate(glm::radians(rotation.x), glm::vec3(1, 0, 0));
		glm::mat4 _rotY = glm::rotate(glm::radians(rotation.y), glm::vec3(0, 1, 0));
		glm::mat4 _rotZ = glm::rotate(glm::radians(rotation.z), glm::vec3(0, 0, 1));
		glm::mat4 rotationMatrix = _rotZ * _rotY * _rotX;

		glm::mat4 scaleMatrix = glm::scale(glm::vec3(scale, scale, scale));

		return translationMatrix * rotationMatrix * scaleMatrix;
	}
		
};