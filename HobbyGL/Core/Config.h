#pragma once

#ifndef CONFIG_H
#define CONFIG_H

#include <string>

class Config
{
public:

	std::string windowName = "HobbyGL";

	unsigned int glMajorVersion = 4;
	unsigned int glMinorVersion = 2;

	unsigned int width = 800;
	unsigned int height = 600;

	/*float backgroundColourR = 0.2f;
	float backgroundColourG = 0.3f;
	float backgroundColourB = 0.3f;*/

	float backgroundColourR = 0.0f;
	float backgroundColourG = 0.0f;
	float backgroundColourB = 0.0f;

};

#endif


