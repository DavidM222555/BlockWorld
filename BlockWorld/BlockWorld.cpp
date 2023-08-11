

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Game.h"

#include <iostream>

// settings


int main()
{
    Game game("test_shader.vert", "test_shader.frag", 1800, 1000);
    game.loop();

    return 0;
}