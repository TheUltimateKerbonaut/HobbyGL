#pragma once

#include <string>

class Config
{
public:

	std::string windowName = "HobbyGL";

	unsigned int glMajorVersion = 4;
	unsigned int glMinorVersion = 3;

	unsigned int width = 800;
	unsigned int height = 600;
	float resolutionScale = 1.0f; // 2.5f for dithering

	bool bloom = true;
	bool dithering = false;

	/*float backgroundColourR = 0.2f;
	float backgroundColourG = 0.3f;
	float backgroundColourB = 0.3f;*/

	float backgroundColourR = 0.0f;
	float backgroundColourG = 0.0f;
	float backgroundColourB = 0.0f;

};


