#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform float offset;

out vec3 ourPosition;

void main()
{
    ourPosition = vec3(aPos.x + offset, -1 * aPos.y, aPos.z);
    gl_Position = vec4(ourPosition, 1.0);
}