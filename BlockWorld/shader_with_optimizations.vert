#version 330 core

layout (location = 0) in float xPos;
layout (location = 1) in float xDirection;

layout (location = 2) in float yPos;
layout (location = 3) in float yDirection;

layout (location = 4) in float zPos;
layout (location = 5) in float zDirection;

layout (location = 6) in float aColor;

out vec3 col;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec3 aPos = vec3(0.0f, 0.0f, 0.0f);

    if (xDirection == 0) {
        aPos.x = xPos + 0.5f;
    } else {
        aPos.x = xPos - 0.5f;
    }

    if (yDirection == 0) {
        aPos.y = yPos + 0.5f;
    } else {
        aPos.y = yPos - 0.5f;
    }

    if (zDirection == 0) {
        aPos.z = zPos + 0.5f;
    } else {
        aPos.z = zPos - 0.5f;
    }   


    gl_Position = projection * view * vec4(aPos, 1.0f);
    col = vec3(0.3f, aColor, 0.0f);
}