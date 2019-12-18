#pragma once

#include <string>

#include <glm/glm.hpp>

class RenderShaderProgram
{
public:
	RenderShaderProgram(std::string VERTEX_SHADER_LOCATION, std::string FRAGMENT_SHADER_LOCATION);

	virtual void connectTextureUnits();

	~RenderShaderProgram();

	void bind();
	void unbind();

protected:

	unsigned int vertexShader;
	unsigned int fragmentShader;
	unsigned int shaderProgram;

	unsigned int getUniformLocation(std::string uniformName);
	virtual void getAllUniformLocations();

	void loadFloat(unsigned int location, float value);
	void loadInt(unsigned int location, int value);
	void loadMat4(unsigned int location, glm::mat4 value);

	void validateLinkingSuccesss(unsigned int programID);
	void validateCompilationSuccesss(unsigned int shaderID, bool isVertexShader, std::string fileName);

	void bindAttribute(unsigned int attribute, std::string attributeName);
	void bindOutAttribute(unsigned int attribute, std::string attributeName);

	virtual void bindAttributes();

	std::string loadShaderFromFile(std::string location);

};

