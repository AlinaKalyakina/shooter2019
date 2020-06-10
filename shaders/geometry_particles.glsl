#version 330

layout(points) in;
layout(line_strip, max_vertices = 2) out;

in VS_OUT {
    vec4 dir;
} gs_in[];
///out vec4 fcolor;

void main() {
    //fcolor = vec4(1., 1., 0., 1.);
    gl_Position = gl_in[0].gl_Position;// + gs_in[0].dir*0.1;
    EmitVertex();
    //fcolor = vec4(1., 1., 1., 1.);
    gl_Position = gl_in[0].gl_Position;// + gs_in[0].dir*0.1;
    EmitVertex();
    EndPrimitive();
}
