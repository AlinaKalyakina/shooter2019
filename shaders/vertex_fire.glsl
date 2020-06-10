#version 330 core

#define SCALE 0.2
#define PI 3.14159265358979323846264
#define TIME_SCALE 1.

layout (location = 0) in vec3 vertex;

out vec4 vertex_color;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform float time;

void main()
{
    vec3 pos = SCALE*vertex*sin(TIME_SCALE*time);
    vertex_color = vec4(0.8, 0.6*abs(cos(TIME_SCALE*time)) + 0.1, (2*TIME_SCALE*time < PI/2 ||
        2*TIME_SCALE*time > 3*PI/2)?0.6*abs(cos(2*TIME_SCALE*time)):0., min(2*abs(cos(TIME_SCALE*time))+0.5, 1.));
    gl_Position = projection * view * model * vec4(pos, 1.0);
    //gl_Position = projection *view*model*vec4(vertex*0.1 + vec3(0., 0., -1.), 1.);
}