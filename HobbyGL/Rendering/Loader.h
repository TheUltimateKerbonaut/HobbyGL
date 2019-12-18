#pragma once

#include "../Core/Mesh.h"
#include "../Core/Texture.h"

#include <vector>
#include <map>

class Loader
{
public:
	Loader();
	~Loader();

	static Mesh loadToVao(std::vector<float> positions, std::vector<unsigned int> indices, std::vector<float> textureCoords);

	static Texture loadTexture(std::string fileName);

private:
	
	static std::map<std::string, unsigned int> textures;
		
	static unsigned int createVAO();
	static void bindIndicesBuffer(std::vector<unsigned int> indices);
	static void storeDataInAttributeList(unsigned int attributeNumber, unsigned int size, std::vector<float> data);
	static void unbindVAO();

};

