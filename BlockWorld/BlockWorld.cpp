

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Game.h"

#include <iostream>

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    Game game("test_shader.vert", "test_shader.frag", 1200, 800);
    game.loop();

    return 0;
}