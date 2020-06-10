#version 330 core

#define MAGNITUDE 40.
#define TIME_SCALE 0.5

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec2 texCoords;
} gs_in[];

out vec2 tex;
out float w;
uniform float time;

vec4 explode(vec4 position, vec3 normal)
{
    vec3 direction = normal * ((sin(TIME_SCALE*time)) / 2.0) * MAGNITUDE;
    return position + vec4(direction, 0.0);
}

vec3 GetNormal()
{
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(a, b));
}

void main() {
    vec3 normal = GetNormal();
    float tmp = max(1-sin(TIME_SCALE*time), 0);
    gl_Position = explode(gl_in[0].gl_Position, normal);
    tex = gs_in[0].texCoords;
    w = tmp;
    EmitVertex();
    gl_Position = explode(gl_in[1].gl_Position, normal);
    tex = gs_in[1].texCoords;
    w = tmp;
    EmitVertex();
    gl_Position = explode(gl_in[2].gl_Position, normal);
    tex = gs_in[2].texCoords;
    w = tmp;
    EmitVertex();
    EndPrimitive();
}