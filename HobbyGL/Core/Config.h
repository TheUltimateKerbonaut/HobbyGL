#pragma once

#include <string>

class Config
{
public:

	std::string windowName = "HobbyGL";

	unsigned int glMajorVersion = 4;
	unsigned int glMinorVersion = 3;

	unsigned int width = 1600;
	unsigned int height = 900;
	//float resolutionScale = 4.0f; // 2.5f for dithering
	//float resolutionScale = width / 320.0f;
	float resolutionScale = 1.0;

	bool bloom = true;
	bool dithering = false;

	bool vsync = true;

	/*float backgroundColourR = 0.2f;
	float backgroundColourG = 0.3f;
	float backgroundColourB = 0.3f;*/

	float backgroundColourR = 0.0f;
	float backgroundColourG = 0.0f;
	float backgroundColourB = 0.0f;

};


