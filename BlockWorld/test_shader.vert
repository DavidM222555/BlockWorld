#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in float aColor;

out vec3 col;

uniform mat4 view;
uniform mat4 projection;

void main()
{

    gl_Position = projection * view * vec4(aPos, 1.0f);
    col = vec3(0.3f, aColor, 0.0f);
}