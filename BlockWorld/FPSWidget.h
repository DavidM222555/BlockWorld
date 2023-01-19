#pragma once

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include <glm/glm.hpp>
#include <stdio.h>

class FPSWidget
{

public:
	FPSWidget(GLFWwindow* window);
	void render(unsigned int fps, glm::vec3 pos, glm::vec3 playerChunk);

};

