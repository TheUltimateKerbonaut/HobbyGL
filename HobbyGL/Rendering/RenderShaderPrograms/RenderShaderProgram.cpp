#include "RenderShaderProgram.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

RenderShaderProgram::RenderShaderProgram(std::string VERTEX_SHADER_LOCATION, std::string FRAGMENT_SHADER_LOCATION)
{
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	std::string text = loadShaderFromFile(VERTEX_SHADER_LOCATION);
	const char *c_str = text.c_str();
	glShaderSource(vertexShader, 1, &c_str, NULL);
	glCompileShader(vertexShader);
	validateCompilationSuccesss(vertexShader, true, VERTEX_SHADER_LOCATION);

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	std::string text1 = loadShaderFromFile(FRAGMENT_SHADER_LOCATION);
	const char *c_str1 = text1.c_str();
	glShaderSource(fragmentShader, 1, &c_str1, NULL);
	glCompileShader(fragmentShader);
	validateCompilationSuccesss(fragmentShader, false, FRAGMENT_SHADER_LOCATION);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	bindAttributes();

	glLinkProgram(shaderProgram);
	validateLinkingSuccesss(shaderProgram);

	getAllUniformLocations();

	connectTextureUnits();


}

void RenderShaderProgram::connectTextureUnits() {}
void RenderShaderProgram::getAllUniformLocations() {}
void RenderShaderProgram::bindAttributes() {}

void RenderShaderProgram::bind()
{
	glUseProgram(shaderProgram);
}

void RenderShaderProgram::unbind()
{
	glUseProgram(0);
}

unsigned int RenderShaderProgram::getUniformLocation(std::string uniformName)
{
	return glGetUniformLocation(shaderProgram, uniformName.c_str());
}

void RenderShaderProgram::loadFloat(unsigned int location, float value)
{
	glUniform1f(location, value);
}

void RenderShaderProgram::loadInt(unsigned int location, int value)
{
	glUniform1i(location, value);
}

void RenderShaderProgram::loadMat4(unsigned int location, glm::mat4 value)
{
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void RenderShaderProgram::validateLinkingSuccesss(unsigned int programID)
{
	int success;
	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetProgramInfoLog(programID, 512, NULL, infoLog);
		std::cerr << "Error: failed to link shader" << infoLog << std::endl;
	}
}

void RenderShaderProgram::validateCompilationSuccesss(unsigned int shaderID, bool isVertexShader, std::string fileName)
{
	int success;
	glGetProgramiv(shaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetProgramInfoLog(shaderID, 512, NULL, infoLog);
		std::cerr << "Error: failed to compile " << (isVertexShader ? "vertex shader" : "fragment shader") << fileName << " " << infoLog << std::endl;
	}
}

void RenderShaderProgram::bindAttribute(unsigned int attribute, std::string attributeName)
{
	glBindAttribLocation(shaderProgram, attribute, attributeName.c_str());
}

void RenderShaderProgram::bindOutAttribute(unsigned int attribute, std::string attributeName)
{
	glBindFragDataLocation(shaderProgram, attribute, attributeName.c_str());
}

std::string RenderShaderProgram::loadShaderFromFile(std::string location)
{
	std::ifstream in(location);
	std::string contents((std::istreambuf_iterator<char>)(in), std::istreambuf_iterator<char>());

	if (contents == "")
	{
		std::cerr << "Error: could not load shader with location " << location << std::endl;
	}

	return contents;
}

RenderShaderProgram::~RenderShaderProgram()
{
	glDetachShader(shaderProgram, vertexShader);
	glDetachShader(shaderProgram, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteProgram(shaderProgram);
}