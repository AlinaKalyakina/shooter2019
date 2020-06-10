#version 440

#define NUM 90
#define ANGLE 2*3.14159265358979323846264/NUM
#define RADIUS 0.2
#define STANDART 480
#define K 1.5

uniform int g_screenWidth;
uniform int g_screenHeight;

layout(points) in;
layout(line_strip, max_vertices = NUM + 9) out;
//layout(line_strip, max_vertices = 2) out;


void main() {
    float x_radius = RADIUS*STANDART/g_screenWidth;
    float y_radius = RADIUS*STANDART/g_screenHeight;
    for (int i = 0; i <= NUM; i++) {
        gl_Position = gl_in[0].gl_Position+vec4((vec2(x_radius*cos(i*ANGLE), y_radius*sin(ANGLE*i))), 0, 0);
        EmitVertex();
    }
    EndPrimitive();
    gl_Position = gl_in[0].gl_Position + vec4(-x_radius*K, 0.0, 0.0, 0.0);
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + vec4( x_radius*K, 0.0, 0.0, 0.0);
    EmitVertex();
    EndPrimitive();

    gl_Position = gl_in[0].gl_Position + vec4(-0.0, -y_radius*K, 0.0, 0.0);
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + vec4( 0.0, y_radius*K, 0.0, 0.0);
    EmitVertex();
    EndPrimitive();
}
