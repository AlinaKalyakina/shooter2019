#version 330

layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 TexCoord;

out vec2 tex;
uniform mat4 model;


void main(void)
{
    tex = TexCoord;
    gl_Position  = model*vec4(pos,0., 1.0);
    gl_Position.z = 0;
}
