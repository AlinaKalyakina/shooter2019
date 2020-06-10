#version 330

layout(location = 0) in vec3 pos;

uniform mat4 projection;
uniform mat4 view;

void main() {
    gl_Position = projection * view * vec4(pos, 1.0);
    gl_PointSize = sin(0.21*pos.z)/2 + sin(0.25*pos.z)/2 + 2;
}
