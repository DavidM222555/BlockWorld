#pragma once

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <string>
#include <future>

#include "NoiseGenerator.h"
#include "World.h"
#include "ChunkManager.h"
#include "Chunk.h"
#include "Shader.h"
#include "Camera.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "BufferLayout.h"
#include "Cube.h"
#include "FPSWidget.h"

#include "stb_image.h"


class Game
{
private:
	GLFWwindow* window;
	Shader shader;
	FPSWidget fpsWidget;
	unsigned int height;
	unsigned int width;

public:
	Game(std::string vertexShaderPath, std::string fragShaderPath, unsigned int screenHeight = 1600, unsigned int screenWidth = 1200);
	
	void clearColor(float r, float g, float b, float a);
	void render(const vector<float>& buffer, const GridCoordinate& chunkPlayerIsIn, const unsigned int currentFPS);
	void loop();
};

void processInput(GLFWwindow* window, World& world);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);