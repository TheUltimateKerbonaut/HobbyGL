#include "Loader.h"

#include <glad/glad.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <map>

std::map<std::string, unsigned int> Loader::textures;

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

unsigned int Loader::createVAO()
{
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	return vao;
}

void Loader::bindIndicesBuffer(std::vector<unsigned int> indices)
{
	unsigned int vboID;
	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices), &indices[0], GL_STATIC_DRAW);
}

void Loader::storeDataInAttributeList(unsigned int attributeNumber, unsigned int size, std::vector<float> data)
{
	unsigned int vboID;
	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(data), &data[0], GL_STATIC_DRAW);
	glVertexAttribPointer(attributeNumber, size, GL_FLOAT, GL_FALSE, size * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
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

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);


		float maxAnisotropy;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy); // 4x anisatropic filtering


		glBindTexture(GL_TEXTURE_2D, 0); // unbind

		stbi_image_free(data);

		textures[fileName] = textureID;

		return Texture(textureID);
	}
	else
	{
		std::cout << "Already have texture " << textures[fileName];
		return Texture(textures[fileName]);
	}
}

Loader::~Loader()
{
}
