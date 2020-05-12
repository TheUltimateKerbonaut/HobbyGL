#include "Debugger.h"

#include <string>
#include <sstream>

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

#include "Logger.h"

std::vector<float> Debugger::performance;

void Debugger::init(Display& display)
{
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(display.window, true);
	ImGui_ImplOpenGL3_Init();
}

void Debugger::renderFrame(Config& config, World& world)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	{
		performance.push_back(ImGui::GetIO().Framerate);
		static float values[100];
		for (int i = 0; i < 100; i++)
		{
			int size = performance.size();

			if (performance.size() >= (unsigned int)(size - i - 1)) values[i] = performance[size - i - 1];
		}

		std::reverse(std::begin(values), std::end(values));
		std::ostringstream title;
		title << "FPS: " << (int)ImGui::GetIO().Framerate << " (" << (int)(1000.0f / ImGui::GetIO().Framerate) << "ms)";

		ImGui::Begin("HobbyGL Debugging Window");

		ImGui::PlotLines(title.str().c_str(), values, IM_ARRAYSIZE(values));

		ImGui::NewLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Graphical options");
		ImGui::Checkbox("Vsync", &config.vsync);
		ImGui::Checkbox("Bloom", &config.bloom);
		ImGui::Checkbox("Dithering", &config.dithering);
		ImGui::Checkbox("Depth of field", &config.depthOfField);
		ImGui::Checkbox("Chromatic abberation", &config.chromaticAbberation);
		ImGui::SliderFloat("Resolution scale", &config.resolutionScale, 0.5f, config.width / 320.0f);

		ImGui::NewLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Camera options");
		ImGui::SliderFloat("Focus distance", &world.camera.focusDistance, 0.0f, 2.0f);
		ImGui::SliderFloat("Apeture", &world.camera.apeture, 0.0f, 1.0f);
		ImGui::SliderFloat("Focal length", &world.camera.focalLength, 0.0f, 15.0f);
		ImGui::SliderFloat("Focal range", &world.camera.focusRange, 0.0f, 2.0f);
		ImGui::SliderFloat("Pitch", &world.camera.pitch, -180.0f, 180.0f);

		ImGui::NewLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Console output");
		ImGui::BeginChild("Scrolling");

		for (unsigned int i = 0; i < Logger::logs.size(); i++)
		{
			if (Logger::logs[i].isError)
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), Logger::logs[i].theMessage.c_str());
			else
				ImGui::Text(Logger::logs[i].theMessage.c_str());
		}
		ImGui::SetScrollHereY(1.0f);
		ImGui::EndChild();

		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Debugger::close()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}