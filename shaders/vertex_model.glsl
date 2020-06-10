#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 TexCoord;

out vec2 tex;
out float w;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;


void main(void)
{
    tex = TexCoord;
    gl_Position  = projection*view*model*vec4(pos, 1.0);
    w = 1.f;
}
