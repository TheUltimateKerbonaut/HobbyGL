#pragma once

#include "Mesh.h"
#include "Texture.h"
#include "Transform.h"

class Sprite
{
public:
	
	inline Sprite(Mesh _mesh, Texture _texture, Transform _transform)
	{
		mesh = _mesh;
		texture = _texture;
		transform = _transform;
	}

	Mesh mesh;
	Texture texture;
	Transform transform;

};
