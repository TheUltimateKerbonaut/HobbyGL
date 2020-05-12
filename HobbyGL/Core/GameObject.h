#pragma once

#include <string>

#include "Mesh.h"
#include "Texture.h"
#include "Transform.h"
#include "../Rendering/Loader.h"

#include "Reflection.h"

#include <iostream>

class GameObject
{
public:

	Mesh mesh;
	Texture texture;
	Transform transform;

	Texture normalMap;
	bool hasNormalMap;

	Texture specularMap;
	bool hasSpecularMap;

	float specularFactor;

	float textureTiling;

	Reflection reflection;
	bool hasReflection= false;

	GameObject(Mesh _mesh, Texture _texture, Transform _transform)
	{
		mesh = _mesh;
		texture = _texture;
		transform = _transform;
	}

	GameObject(const std::string& _mesh, const std::string& _texture)
	{
		mesh = Loader::loadToVao(_mesh, false);
		texture = Loader::loadTexture(_texture);
		transform = Transform();
		hasNormalMap = false;
		hasSpecularMap = false;
		specularFactor = 0.0f;
		textureTiling = 1.0f;
	}

	GameObject(const std::string& _mesh, const std::string& _texture, const std::string& _normalMap)
	{
		mesh = Loader::loadToVao(_mesh, true);
		texture = Loader::loadTexture(_texture);
		transform = Transform();

		normalMap = Loader::loadTexture(_normalMap);
		hasNormalMap = true;
		hasSpecularMap = false;
		specularFactor = 0.0f;
		textureTiling = 1.0f;
	}

	GameObject(const std::string& _mesh, const std::string& _texture, const std::string& _normalMap, const std::string& _specularMap)
	{
		mesh = Loader::loadToVao(_mesh, true);
		texture = Loader::loadTexture(_texture);
		transform = Transform();

		normalMap = Loader::loadTexture(_normalMap);
		hasNormalMap = true;

		specularMap = Loader::loadTexture(_specularMap);
		hasSpecularMap = true;
		specularFactor = 1.0f;
		textureTiling = 1.0f;
	}

	void addReflection()
	{
		reflection = Reflection(transform.position);
		hasReflection = true;
	}

	~GameObject()
	{
		if (hasReflection) reflection.destroyFBO();
	}

};