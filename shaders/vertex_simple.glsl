#version 330

layout(location = 0) in vec3 pos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec4 color;

out vec4 vertex_color;

void main(void)
{
    gl_Position  = projection*view*model*vec4(pos, 1.0);
    vertex_color = color;
}
