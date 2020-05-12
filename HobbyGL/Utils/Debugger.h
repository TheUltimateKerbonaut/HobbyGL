#pragma once

#include "../Core/Display.h"
#include "../Core/Config.h"
#include "../Core/World.h"

#include <vector>

class Debugger
{

public:
	static void init(Display& display);

	static void renderFrame(Config& config, World& world);

	static void close();

	static std::vector<float> performance;

};

