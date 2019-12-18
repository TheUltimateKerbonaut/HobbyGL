#pragma once
class Texture
{
public:

	inline Texture() {}

	inline Texture(unsigned int _id)
	{
		textureID = _id;
	}

	unsigned int textureID;
};