#pragma once

#include <glad/glad.h>

class Mesh
{
public:

	Mesh()
	{

	}
	
	inline Mesh(unsigned int _vaoID, unsigned int _vertexCount)
	{
		vaoID = _vaoID;
		vertexCount = _vertexCount;
	}

	unsigned int vaoID;
	unsigned int vertexCount;

};