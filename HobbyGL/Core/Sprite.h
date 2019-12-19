#pragma once

#include <vector>
#include <string>

#include "Mesh.h"
#include "Texture.h"
#include "Transform.h"
#include "../Rendering/Loader.h"

class Sprite
{
public:
	
	const std::vector<float> vertices = {
	 0.5f,  0.5f, 0.0f,  // top right
	 0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f,  // bottom left
	-0.5f,  0.5f, 0.0f   // top left 
	};
	const std::vector<unsigned int> indices = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};
	const std::vector<float> textureCoords = {
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f
	};

	static bool didLoadVAO;
	static Mesh standardMesh;

	inline Sprite(Mesh _mesh, Texture _texture, Transform _transform)
	{
		mesh = _mesh;
		texture = _texture;
		transform = _transform;

		transform.scale = 500;
	}

	inline Sprite(const std::string& _texture)
	{
		if (didLoadVAO)
		{
			mesh = standardMesh;
			mesh.vaoID = standardMesh.vaoID;
			mesh.vertexCount = standardMesh.vertexCount;
		}
		else                
		{
			mesh = Loader::loadToVao(vertices, indices, textureCoords);
			standardMesh = mesh;
			standardMesh.vaoID = mesh.vaoID;
			standardMesh.vertexCount = mesh.vertexCount;
			didLoadVAO = true;
		}
		texture = Loader::loadTexture(_texture);
		transform = Transform();

		transform.scale = 500;
	}

	Mesh mesh;
	Texture texture;
	Transform transform;

};