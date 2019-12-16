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
	
	float x;
	float y;
	float z;

	float scale;

	float rotX;
	float rotY;
	float rotZ;

	inline Transform()
	{
		x = 0;
		y = 0;
		z = 0;
		scale = 1;
		rotX = 0;
		rotY = 0;
		rotZ = 0;
	}

	inline glm::mat4 getMatrix()
	{
		glm::mat4 translationMatrix = glm::translate(glm::vec3(x, y, z));

		glm::mat4 _rotX = glm::rotate(glm::radians(rotX), glm::vec3(1, 0, 0));
		glm::mat4 _rotY = glm::rotate(glm::radians(rotY), glm::vec3(0, 1, 0));
		glm::mat4 _rotZ = glm::rotate(glm::radians(rotZ), glm::vec3(0, 0, 1));
		glm::mat4 rotationMatrix = _rotZ * _rotY * _rotX;

		glm::mat4 scaleMatrix = glm::scale(glm::vec3(scale, scale, scale));

		return translationMatrix * rotationMatrix * scaleMatrix;
	}
		
};