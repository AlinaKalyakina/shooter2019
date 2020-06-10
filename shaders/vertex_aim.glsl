#version 330 core
layout (location = 0) in vec2 aim_pos;

void main()
{
    gl_Position = vec4(aim_pos, 0.0, 1.0);
}