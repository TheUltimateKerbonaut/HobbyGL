#include "Loader.h"

#include <glad/glad.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"

#include <map>

std::map<std::string, TextureMap> Loader::textures;
std::vector<unsigned int> Loader::VAOs;
std::vector<unsigned int> Loader::VBOs;
std::vector<unsigned int> Loader::Textures;

Loader::Loader()
{

}

Mesh Loader::loadToVao(std::vector<float> positions, std::vector<unsigned int> indices, std::vector<float> textureCoords)
{
	unsigned int vaoID = createVAO();
	
	bindIndicesBuffer(indices);
	
	storeDataInAttributeList(0, 3, positions);
	storeDataInAttributeList(1, 2, textureCoords);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	unbindVAO();
	
	return Mesh(vaoID, indices.size());

}

Mesh Loader::loadToVao(std::vector<float> positions, std::vector<float> normals, std::vector<unsigned int> indices, std::vector<float> textureCoords)
{
	unsigned int vaoID = createVAO();

	bindIndicesBuffer(indices);

	storeDataInAttributeList(0, 3, positions);
	storeDataInAttributeList(1, 3, normals);
	storeDataInAttributeList(2, 2, textureCoords);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	unbindVAO();

	return Mesh(vaoID, indices.size());

}

unsigned int Loader::createVAO()
{
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	VAOs.push_back(vao);
	return vao;
}

void Loader::bindIndicesBuffer(std::vector<unsigned int> indices)
{
	unsigned int vboID;
	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices.front(), GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	VBOs.push_back(vboID);
}

void Loader::storeDataInAttributeList(unsigned int attributeNumber, unsigned int size, std::vector<float> data)
{
	unsigned int vboID;
	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(attributeNumber, size, GL_FLOAT, GL_FALSE, size * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	VBOs.push_back(vboID);
}

void Loader::unbindVAO()
{
	glBindVertexArray(0);
}

Texture Loader::loadTexture(std::string fileName)
{
	if (textures.find(fileName) == textures.end())
	{

		int width, height, nrChannels;
		unsigned char *data = stbi_load(("res/" + fileName).c_str(), &width, &height, &nrChannels, 0);
		if (!data)
		{
			std::cerr << "Failed to load texture " << fileName.c_str() << std::endl;
		}

		unsigned int textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexImage2D(GL_TEXTURE_2D, 0, (nrChannels == 3) ? GL_RGB : GL_RGBA, width, height, 0, (nrChannels == 3) ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);

		float maxAnisotropy;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy); // 4x anisatropic filtering

		glBindTexture(GL_TEXTURE_2D, 0); // unbind

		stbi_image_free(data);

		textures[fileName] = TextureMap(textureID, width, height);

		Textures.push_back(textureID);
		return Texture(textureID);
	}
	else
	{
		return Texture(textures[fileName].textureID);
	}
}

TextureReturn Loader::loadTextureAndGetDimensions(std::string fileName)
{
	if (textures.find(fileName) == textures.end())
	{

		int width, height, nrChannels;
		unsigned char *data = stbi_load(("res/" + fileName).c_str(), &width, &height, &nrChannels, 0);
		if (!data)
		{
			std::cerr << "Failed to load texture " << fileName.c_str() << std::endl;
		}

		unsigned int textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexImage2D(GL_TEXTURE_2D, 0, (nrChannels == 3) ? GL_RGB : GL_RGBA, width, height, 0, (nrChannels == 3) ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);

		float maxAnisotropy;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy); // 4x anisatropic filtering

		glBindTexture(GL_TEXTURE_2D, 0); // unbind

		stbi_image_free(data);

		textures[fileName] = TextureMap(textureID, width, height);

		Textures.push_back(textureID);
		return TextureReturn(Texture(textureID), width, height);
	}
	else
	{
		return TextureReturn(Texture(textures[fileName].textureID), textures[fileName].width, textures[fileName].height);
	}
}

Mesh Loader::loadToVao(const std::string& fileName)
{
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<unsigned int> indices;
	std::vector<float> textureCoords;

	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(("res/" + fileName + ".obj").c_str(), aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_OptimizeMeshes | aiProcess_FlipUVs);
	
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cerr << "Error: Failed to load model " << fileName << "!" << std::endl;
		return Mesh();
	}
	else
	{
		processNode(scene->mRootNode, scene, vertices, normals, indices, textureCoords);
		return loadToVao(vertices, normals, indices, textureCoords);
	}
}

void Loader::processNode(aiNode *node, const aiScene *scene, std::vector<float>& vertices, std::vector<float>& normals, std::vector<unsigned int>& indices, std::vector<float>& textureCoords)
{
	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[i];
		processMesh(mesh, scene, vertices, normals, indices, textureCoords);
	}
}

void Loader::processMesh(aiMesh *mesh, const aiScene *scene, std::vector<float>& vertices, std::vector<float>& normals, std::vector<unsigned int>& indices, std::vector<float>& textureCoords)
{
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{

		vertices.push_back(mesh->mVertices[i].x);
		vertices.push_back(mesh->mVertices[i].y);
		vertices.push_back(mesh->mVertices[i].z);

		normals.push_back(mesh->mNormals[i].x);
		normals.push_back(mesh->mNormals[i].y);
		normals.push_back(mesh->mNormals[i].z);


		if (mesh->mTextureCoords[0])
		{
			textureCoords.push_back(mesh->mTextureCoords[0][i].x);
			textureCoords.push_back(mesh->mTextureCoords[0][i].y);
		}
		else
		{
			std::cerr << "Eror: Model does no have texture coordinates!" << std::endl;
		}
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
}

void Loader::close()
{
	for (unsigned int x : VBOs)
		glDeleteBuffers(1, &x);
	for (unsigned int x : VAOs)
		glDeleteVertexArrays(1, &x);
	for (unsigned int x : Textures)
		glDeleteTextures(1, &x);
}

Loader::~Loader()
{
	
}
