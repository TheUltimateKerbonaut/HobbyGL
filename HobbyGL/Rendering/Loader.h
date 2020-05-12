#pragma once

#include "../Core/Mesh.h"
#include "../Core/Texture.h"

#include <vector>
#include <map>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct TextureReturn
{
	Texture texture;
	int width;
	int height;

	TextureReturn(Texture texture, int width, int height)
	{
		this->texture = texture;
		this->width = width;
		this->height = height;
	}
};

struct TextureMap
{
	unsigned int textureID;
	int width;
	int height;

	TextureMap(unsigned int textureID, int width, int height)
	{
		this->textureID = textureID;
		this->width = width;
		this->height = height;
	}

	TextureMap()
	{

	}
};

class Loader
{
public:

	Loader();
	~Loader();

	static Mesh loadToVao(std::vector<float> positions, std::vector<unsigned int> indices, std::vector<float> textureCoords);
	static Mesh loadToVao(std::vector<float> positions, std::vector<float> normals, std::vector<unsigned int> indices, std::vector<float> textureCoords);
	static Mesh loadToVao(std::vector<float> positions, std::vector<float> normals, std::vector<unsigned int> indices, std::vector<float> textureCoords, std::vector<float> tangents);
	static Mesh loadToVao(const std::string& fileName, bool hasTangents);

	static Texture loadTexture(std::string fileName);
	static Texture loadCubemapTexture(std::string fileNames[6]);
	static TextureReturn loadTextureAndGetDimensions(std::string fileName);

	static void close();

private:
	
	static std::map<std::string, TextureMap> textures;
		
	static unsigned int createVAO();
	static void bindIndicesBuffer(std::vector<unsigned int> indices);
	static void storeDataInAttributeList(unsigned int attributeNumber, unsigned int size, std::vector<float> data);
	static void unbindVAO();

	static std::vector<unsigned int> VAOs;
	static std::vector<unsigned int> VBOs;
	static std::vector<unsigned int> Textures;

	static void processNode(aiNode *node, const aiScene *scene, std::vector<float>& vertices, std::vector<float>& normals, std::vector<unsigned int>& indices, std::vector<float>& textureCoords, std::vector<float>& tangents, bool hasTangents);
	static void processMesh(aiMesh *mesh, const aiScene *scene, std::vector<float>& vertices, std::vector<float>& normals, std::vector<unsigned int>& indices, std::vector<float>& textureCoords, std::vector<float>& tangents, bool hasTangents);

};

